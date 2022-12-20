    /*******************************************************************************
   Forked from original mjs_firmware: Copyright (c) 2016 Thomas Telkamp, Matthijs Kooijman, Bas Peschier, Harmen Zijp
   Code for measuring soil moisture added by Paul Brouwer

   Permission is hereby granted, free of charge, to anyone
   obtaining a copy of this document and accompanying files,
   to do whatever they want with them without any restriction,
   including, but not limited to, copying, modification and redistribution.
   NO WARRANTY OF ANY KIND IS PROVIDED.

   In order to compile the following libraries need to be installed:
   - SparkFunHTU21D: https://github.com/sparkfun/SparkFun_HTU21D_Breakout_Arduino_Library
   - NeoGPS (mjs-specific fork): https://github.com/meetjestad/NeoGPS
   - Adafruit_SleepyDog: https://github.com/adafruit/Adafruit_SleepyDog
   - lmic (mjs-specific fork): https://github.com/meetjestad/arduino-lmic
 *******************************************************************************/

//Version flex V0: this version is used to implement the TTL-UART on V2 soil moisture sensors


// include external libraries
#include <SPI.h>
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <SoftwareSerial.h>
#include <NMEAGPS.h>
#include <Adafruit_SleepyDog.h>
#include <avr/power.h>
#include <util/atomic.h>

#define DEBUG true
#include "bitstream.h"
#include "mjs_lmic.h"

// Firmware version to send. Should be incremented on release (i.e. when
// signficant changes happen, and/or a version is deployed onto
// production nodes). This value should correspond to a release tag.
// For untagged/experimental versions, use 255.
const uint8_t FIRMWARE_VERSION = 255;

// This sets the ratio of the battery voltage divider attached to A0,
// below works for 100k to ground and 470k to the battery. A setting of
// 0.0 means not to measure the voltage. On first generation boards, this
// should only be enabled when the AREF pin of the microcontroller was
// disconnected.
float const BATTERY_DIVIDER_RATIO = 0.0;
//float const BATTERY_DIVIDER_RATIO = (100.0 + 470.0) / 100.0;

// Enable this define when a light sensor is attached
//#define WITH_LUX

// Enable this define when a soil moisture sensor is attached
#define WITH_SM;

// These values define the sensitivity and calibration of the PAR / Lux
// measurement.
// R12 Reference resistor for low light levels
//  (nominal 100K in Platform Rev 2)
// R11 Reference shunt resistor for high ligh levels
//  (nominal 10K in platform Rev 2)
// Value in Ohms
float const R12 = 100000.0;
// Value in Ohms
float const R11 = 10000.0;

// Reverse light current of the foto diode Ea at 1klx
// uA @ 1000lx  eg 8.9 nA/lx
// The Reverse dark current (max 30 nA ) is neglectable for our purpose
float const light_current = 8.9;

// R11 and R12 in parallel
float const R11_R12 = (R12 * R11) / (R12 + R11);
float const lx_conv_high = 1.0E6 / (R11_R12 * light_current * 1024.0);
float const lx_conv_low = 1.0E6 / (R12 * light_current * 1024.0);

// Value in mV (nominal @ 25ºC, Vcc=3.3V)
// The temperature coefficient of the reference_voltage is neglected
float const reference_voltage_internal = 1137.0;

// setup GPS module
uint8_t const GPS_PIN = 8;

// Sensor object
HTU21D htu;

// Most recently read values
float temperature;
float humidity;

//soil moisture - 4 levels and 1 control
#ifdef WITH_SM
uint8_t const RX_PIN = 7; 
uint8_t const TX_PIN = 6; 
SoftwareSerial Vserial(RX_PIN,TX_PIN);

const int levels=6;
uint16_t l[levels];
uint8_t t[levels];
uint8_t samples;
#endif

uint16_t vcc = 0;
#ifdef WITH_LUX
uint32_t lux = 0;
#endif

int32_t lat24 = 0;
int32_t lng24 = 0;

// define various pins
uint8_t const SW_GND_PIN = 20;
uint8_t const LED_PIN = 21;
uint8_t const LUX_HIGH_PIN = 5;

// setup timing variables
uint32_t const UPDATE_INTERVAL = 1800000; //30m
uint32_t const GPS_TIMEOUT = 120000; //2m
uint32_t const WIRE_TIMEOUT = 1000000; //microseconds
uint32_t const SWG_GND_DELAY=2000; //milliseconds
uint32_t const SOIL_DELAY=10000; //milliseconds (8000-2000 for SWG_GND)
long const TTL_timeout=8000; //ms

// Update GPS position after transmitting this many updates
uint16_t const GPS_UPDATE_RATIO = 24*4;

// When sending extra data, use this many bits to specify the size
// (allows up to 32-bit values)
uint8_t const EXTRA_SIZE_BITS = 5;

enum {
  FLAG_WITH_LUX = (1 << 7),
  FLAG_WITH_PM = (1 << 6),
  FLAG_WITH_BATTERY = (1 << 5),
  // bits 4:1 reserved for future additions
  FLAG_WITH_EXTRA = (1 << 0),
};

uint32_t lastUpdateTime = 0;
uint32_t updatesBeforeGpsUpdate = 0;
gps_fix gps_data;

uint8_t const LORA_PORT = 13;

void setup() {
  
  // when in debugging mode start serial connection
  if(DEBUG) {
    Serial.begin(9600);
  }

  // setup LoRa transceiver
  mjs_lmic_setup();

  // setup switched ground and power down connected peripherals (GPS module)
  pinMode(SW_GND_PIN, OUTPUT);
  digitalWrite(SW_GND_PIN, LOW);

  // This pin can be used in OUTPUT LOW mode to add an extra pulldown
  // resistor, or in INPUT mode to keep it disconnected
  pinMode(LUX_HIGH_PIN, INPUT);

  // blink 'hello'
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  
}

void loop() {
  // We need to calculate how long we should sleep, so we need to know how long we were awake
  unsigned long startMillis = millis();

  // Activate GPS every now and then to update our position
  if (updatesBeforeGpsUpdate == 0) {
    getPosition();
    updatesBeforeGpsUpdate = GPS_UPDATE_RATIO;
    // Use the lowest datarate, to maximize range. This helps for
    // debugging, since range problems can be more easily distinguished
    // from other problems (lockups, downlink problems, etc).
    LMIC_setDrTxpow(DR_SF12, 14);
  } else {
    LMIC_setDrTxpow(DR_SF9, 14);
  }
  updatesBeforeGpsUpdate--;

  //switch ground pin on
  digitalWrite(SW_GND_PIN, HIGH); 
  delay(SWG_GND_DELAY); //time for sensor to start-up
  
  // Activate and read our I2C sensor
  htu.begin(); //includes a wire.begin in the sparfun library moved here from setup
  temperature = htu.readTemperature();
  humidity = htu.readHumidity();
  vcc = readVcc();
  Wire.end(); // to enable SDA & SCL to be used by Vserial

#ifdef WITH_SM
  delay(SOIL_DELAY);
  
  if(beginSoilMoisture()){
    readSoilMoisture();
  }
  else{
    //set values to zero
    for(int lev=0;lev<levels;lev++){
      l[lev]=0;
      t[lev]=0;
    }
  }
  
  endSoilMoisture();

#endif // WITH_SM

  //switch ground pin off  
  digitalWrite(SW_GND_PIN, LOW); 
  
  
#ifdef WITH_LUX
  lux = readLux();
#endif // WITH_LUX

  if (DEBUG){
    dumpData();
  }

  // Work around a race condition in LMIC, that is greatly amplified
  // if we sleep without calling runloop and then queue data
  // See https://github.com/lmic-lib/lmic/issues/3
  os_runloop_once();

  // We can now send the data
  queueData();

  mjs_lmic_wait_for_txcomplete();

  // Schedule sleep
  unsigned long msPast = millis() - startMillis;
  unsigned long sleepDuration = UPDATE_INTERVAL;
  if (msPast < sleepDuration)
    sleepDuration -= msPast;
  else
    sleepDuration = 0;

  if (DEBUG) {
    Serial.print(F("Slp "));
    Serial.print(sleepDuration);
    Serial.println(F("ms.."));
    Serial.flush();
  }
  doSleep(sleepDuration);
  if (DEBUG) {
    Serial.println(F("Woke"));
  }
}

void doSleep(uint32_t time) {
  ADCSRA &= ~(1 << ADEN);
  power_adc_disable();

  while (time > 0) {
    uint16_t slept;
    if (time < 8000)
      slept = Watchdog.sleep(time);
    else
      slept = Watchdog.sleep(8000);

    // Update the millis() and micros() counters, so duty cycle
    // calculations remain correct. This is a hack, fiddling with
    // Arduino's internal variables, which is needed until
    // https://github.com/arduino/Arduino/issues/5087 is fixed.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      extern volatile unsigned long timer0_millis;
      extern volatile unsigned long timer0_overflow_count;
      timer0_millis += slept;
      // timer0 uses a /64 prescaler and overflows every 256 timer ticks
      timer0_overflow_count += microsecondsToClockCycles((uint32_t)slept * 1000) / (64 * 256);
    }

    if (slept >= time)
      break;
    time -= slept;
  }

  power_adc_enable();
  ADCSRA |= (1 << ADEN);
}

void dumpData() {
  //disabled due to program storage place limitations
  //if (gps_data.valid.location && gps_data.valid.status && gps_data.status >= gps_fix::STATUS_STD) {
    //Serial.print(F("lat/lon: "));
    //Serial.print(gps_data.latitudeL()/10000000.0, 6);
    //Serial.print(F(","));
    //Serial.println(gps_data.longitudeL()/10000000.0, 6);
  //} else {
    //Serial.println(F("No GPS"));
  //}

  Serial.print(F("t="));
  Serial.print(temperature, 1);
  //Serial.print(F(", h="));
  //Serial.print(humidity, 1);
  //Serial.print(F(", v="));
  //Serial.print(vcc, 1);


#ifdef WITH_LUX
  Serial.print(F(", lux="));
  Serial.print(lux);
#endif // WITH_LUX


#ifdef WITH_SM
  for(int lev=0;lev<levels;lev++){;
    Serial.print(lev);
    Serial.print(F(" = "));
    Serial.print(l[lev]);
    Serial.print(F(" | "));
    Serial.print(t[lev]);
    Serial.print(F("\n"));
  }
#endif // WITH_SM

  Serial.println();
  Serial.flush();
}

void getPosition()
{
  // Setup GPS
  SoftwareSerial gpsSerial(GPS_PIN, GPS_PIN);
  NMEAGPS gps;

  gpsSerial.begin(9600);
  memset(&gps_data, 0, sizeof(gps_data));
  gps.reset();
  gps.statistics.init();

  digitalWrite(SW_GND_PIN, HIGH);

  // Empty serial input buffer, so only new characters are processed
  while(Serial.read() >= 0) /* nothing */;

  if (DEBUG)
    Serial.println(F("Waiting for GPS, send 's' to skip..."));

  unsigned long startTime = millis();
  uint8_t valid = 0;
  while (millis() - startTime < GPS_TIMEOUT && valid < 10) {
    if (gps.available(gpsSerial)) {
      gps_data = gps.read();
      if (gps_data.valid.location && gps_data.valid.status && gps_data.status >= gps_fix::STATUS_STD) {
        valid++;
        lat24 = int32_t((int64_t)gps_data.latitudeL() * 32768 / 10000000);
        lng24 = int32_t((int64_t)gps_data.longitudeL() * 32768 / 10000000);
      } else {
        lat24 = 0;
        lng24 = 0;
      }
      if (gps_data.valid.satellites) {
        Serial.print(F("Satellites: "));
        Serial.println(gps_data.satellites);
      }
    }
    if (DEBUG && tolower(Serial.read()) == 's')
      break;
  }
  digitalWrite(SW_GND_PIN, LOW);

  if (gps.statistics.ok == 0)
    Serial.println(F("No GPS data received, check wiring"));

  gpsSerial.end();
}

void queueData() {
  uint8_t length = 12;
  uint8_t flags = 0;

  if (BATTERY_DIVIDER_RATIO) {
    flags |= FLAG_WITH_BATTERY;
    length += 1;
  }

#ifdef WITH_LUX
  flags |= FLAG_WITH_LUX;
  length += 2;
#endif

  // To add extra data, uncomment the section below and change the number
  // of extra data bits to what you are actualy using. Additionally,
  // uncomment a bit of code further down that actually adds the data to
  // the packet, and also shows how the number of bits is counted.

  const uint8_t extra_bits = 6*(EXTRA_SIZE_BITS+16)+6*(EXTRA_SIZE_BITS+8);
  length += (extra_bits + 7)/8;
  flags |= FLAG_WITH_EXTRA;

  uint8_t data[length];
  BitStream packet(data, sizeof(data));

  packet.append(flags, 8);

  packet.append(FIRMWARE_VERSION, 8);

  packet.append(lat24, 24);
  packet.append(lng24, 24);

  // pack temperature and humidity
  int16_t tmp16 = temperature * 16;
  packet.append(tmp16, 12);

  int16_t hum16 = humidity * 16;
  packet.append(hum16, 12);

  // Encoded in units of 10mv, starting at 1V
  uint8_t vcc8 = (vcc - 1000) / 10;
  packet.append(vcc8, 8);

#ifdef WITH_LUX
  // Chop off 2 bits to allow up to 256k lux (maximum solar power should be around 128k)
  packet.append(lux >> 2, 16);
#endif

  if (BATTERY_DIVIDER_RATIO) {
    analogReference(INTERNAL);
    uint16_t reading = analogRead(A0);
    // Encoded in units of 20mv
    uint8_t batt = (uint32_t)(50*BATTERY_DIVIDER_RATIO*1.1)*reading/1023;
    // Shift down, zero means 1V now
    if (batt >= 50)
      packet.append(batt - 50, 8);
    else
      packet.append(0, 8);
  }
  // Uncomment this section to add extra data. The example below adds a
  // 10-bit value followed by a 1 bit value. Each extra field is
  // transmitted as a 6-bit size field, followed by a (size+1)-bits value
  // field.
  //
  // Do not forget to uncomment the block around `extra_bits` a bit
  // further up as well.
  
  // This uses some random values, replace these variables by your values.

  // Capacity values 
  for (int i=0;i<levels;i++){
      packet.append(16-1, EXTRA_SIZE_BITS);
      packet.append(l[i], 16);
      packet.append(8-1, EXTRA_SIZE_BITS);
      packet.append(t[i], 8);
  }
  
  // Fill any remaining bits (from rounding up to whole bytes) with 1's,
  // so they cannot be a valid field.
  packet.append(0xff, packet.free_bits());

  // Prepare upstream data transmission at the next possible time.
  LMIC_setTxData2(LORA_PORT, packet.data(), packet.byte_size(), 0);
  if (DEBUG)
  {
    Serial.println(F("Packet queued"));
    uint8_t *data = packet.data();
    for (int i = 0; i < packet.byte_size(); i++)
    {
      if (data[i] < 0x10)
        Serial.write('0');
      Serial.print(data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.flush();
  }
}

uint16_t readVcc()
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  // Wait a bit before measuring to stabilize the reference (or
  // something).  The datasheet suggests that the first reading after
  // changing the reference is inaccurate, but just doing a dummy read
  // still gives unstable values, but this delay helps. For some reason
  // analogRead (which can also change the reference) does not need
  // this.
  delay(2);

  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  uint16_t result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

#ifdef WITH_LUX
uint32_t readLux()
{
  uint32_t result = 0;
  uint8_t range = 0;

  // Set the Reference Resistor to just R12
  pinMode(LUX_HIGH_PIN, INPUT);
  // Read the value of Analog input 2 against the internal reference
  analogReference(INTERNAL);
  // Throw away the first reference, in case the internal reference
  // still needs to start up and stabilize (datasheet recommendation)
  analogRead(A2);
  uint16_t raw_adc = analogRead(A2);
  // Check if read_low has an overflow
  if (raw_adc < 1000)
  {
    result = uint32_t(lx_conv_low * reference_voltage_internal * raw_adc);
    range = 1;
  } else {
    // Set the Reference Resistor to R11 parallel with R12 for more range
    pinMode(LUX_HIGH_PIN, OUTPUT);
    digitalWrite(LUX_HIGH_PIN, LOW);

    // An external capacitor can be added to charge the ADC internal 14pF
    // without dropping significant voltage, improving read values at low
    // values. If the capacitance is 1000x as big as the internal
    // capacitance, the drop should be limited to 1 ADC value, so 10nF
    // should be fine, but in practice still shows ±50 ADC counts of
    // deviation. Using 100nF reduces this to ±15, so we're using that.
    // Possible there are more sources of noise than just the internal
    // capacitance.
    //
    // When switching from R12 to R11+R12, the capacitor has to charge
    // through R11+R12, which has an RC time of 100nF x 9k = 900μs. To be
    // sure, we wait for 3ms.
    delay(3);

    raw_adc = analogRead(A2);
    // Check if read_high has an overflow
    if (raw_adc < 1000)
    {
      result = uint32_t(lx_conv_high * reference_voltage_internal * raw_adc);
      range = 2;
    } else {
      // Read the value of Analog input 2 against VCC for more range
      analogReference(DEFAULT);
      raw_adc = analogRead(A2);
      result = uint32_t(lx_conv_high * vcc * raw_adc);
      range = 3;
    }
  }

  // Set the Reference Resistor to 100K to draw the least current
  pinMode(LUX_HIGH_PIN, INPUT);
  if (DEBUG)
  {
    Serial.print(F("Lux_reading : "));
    Serial.print(result);
    Serial.print(F(" lx, range="));
    Serial.print(range);
    Serial.print(F(", adc="));
    Serial.println(raw_adc);
  }
  return result;
}

#endif

#ifdef WITH_SM
bool beginSoilMoisture(){

  //Start virtual serial connection
  Vserial.begin(9600);

  //wait for Vserial with a timeout of 1 second
  int c=0;
  while(!Vserial && c<100){
    delay(10);
    c++;
  }

  if(c>=100){
    return false;
  }
  else 
    return true;
  
}

void endSoilMoisture(){
  Vserial.end();
}

void readSoilMoisture(){

  //bytes that are expected
  const int byte_num = 19;
  uint8_t rec_buffer[byte_num+1];

  union { //just used to convert uint16_t to uint8_t, this could be done more elegantly
    uint8_t b[2];
    uint16_t i;
  } C;

  //time-out parameters
  bool succeed=false;
  long TTL_time=0;
  long start_time=millis();
  uint8_t checksum_slave;
  uint8_t checksum_master;

  while(!succeed){
    // put your main code here, to run repeatedly:
    Vserial.listen();
    
    int n=0;  
    while(Vserial.available()>0){
      rec_buffer[n]=Vserial.read();
      n++;
    }
    delay(100); //nodig anders gaat t niet goed om  een reden
    
    //collect data
    if(n>0){

      Serial.println(n);
  
      //if number of bytes match
      if(n==byte_num+1){
  
        //calculate the checksum
        checksum_slave=rec_buffer[byte_num];
        checksum_master=0;
        
        for(int i=0;i<byte_num;i++){
          checksum_master+=rec_buffer[i];
  
          //print all bytes
          //Serial.print(i);
          //Serial.print(F(" : "));
          //Serial.print(rec_buffer[i]);
          //Serial.print(F("\n"));
        }

        samples=rec_buffer[byte_num-1];

        //set success
        if(checksum_master==checksum_slave){
          succeed=true;

          for(int lev=0;lev<levels;lev++){
            C.b[0]=rec_buffer[3*lev];
            C.b[1]=rec_buffer[3*lev+1];
            l[lev]=float(pow(10,8))/float(C.i); // 10nF
            t[lev]=rec_buffer[3*lev+2];
          }
        
         }
      } //if n==byte_num
      
    } //if n>0

    //cause a time-out after period x
    if(!succeed){
      TTL_time=millis()-start_time;
    }
    
    if(TTL_time>TTL_timeout){
      succeed=true;
      Serial.print(F("Time-out"));

      //set values to zero
      for(int lev=0;lev<levels;lev++){
        l[lev]=0;
        t[lev]=0;
      }
    }

  } //while loop

  if(TTL_time<TTL_timeout){
    Serial.print(F("success, time: "));
  } 
  Serial.print(TTL_time);
  Serial.print("\n");

} //readSoilMoisture
#endif
