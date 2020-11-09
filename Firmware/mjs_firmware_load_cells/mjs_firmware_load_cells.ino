  /*******************************************************************************
   Copyright (c) 2016 Thomas Telkamp, Matthijs Kooijman, Bas Peschier, Harmen Zijp

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

// include external libraries
#include <SPI.h>
#include <Wire.h>
#include <SparkFunHTU21D.h>
#include <SoftwareSerial.h>
#include <NMEAGPS.h>
#include <Adafruit_SleepyDog.h>
#include <avr/power.h>
#include <util/atomic.h>
#include <Adafruit_ADS1015.h>


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

// Value in mV (nominal @ 25ºC, Vcc=3.3V)
// The temperature coefficient of the reference_voltage is neglected
float const reference_voltage_internal = 1137.0;

// setup GPS module
uint8_t const GPS_PIN = 8;

// Sensor objects
HTU21D htu;
Adafruit_ADS1115 ads;

// sensor values
float temperature;
float humidity;
uint8_t const ch=1; //load cell
uint16_t ADS[ch]; //load cell
uint16_t vcc = 0;
int32_t lat24 = 0;
int32_t lng24 = 0;

// define various pins
uint8_t const SW_GND_PIN = 20;
uint8_t const LED_PIN = 21;
uint8_t const LUX_HIGH_PIN = 5;

// setup timing variables
uint32_t const UPDATE_INTERVAL = 900000; //30m
uint32_t const GPS_TIMEOUT = 120000; //2m
uint32_t const WIRE_TIMEOUT = 1000000; //microseconds
uint32_t const SWG_GND_DELAY=2000; //milliseconds
uint32_t const SOIL_DELAY=10000; //milliseconds

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

  LMIC_setDrTxpow(DR_SF9, 14);

  //switch ground pin on
  digitalWrite(SW_GND_PIN, HIGH); 
  delay(SWG_GND_DELAY); //time for sensor to start-up
  
  // Activate and read our sensorss
  htu.begin(); //includes a wire.begin in the sparfun library moved here from setup
  temperature = htu.readTemperature();
  humidity = htu.readHumidity();
  vcc = readVcc();

  readLoadCell();

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
  Serial.print(F(", h="));
  Serial.print(humidity, 1);
  Serial.print(F(", v="));
  Serial.print(vcc, 1);

#ifdef WITH_LUX
  Serial.print(F(", lux="));
  Serial.print(lux);
#endif // WITH_LUX

  Serial.println();
  Serial.flush();
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

  const uint8_t extra_bits = ch*(EXTRA_SIZE_BITS+16);
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

  // LOAD cell values 
  packet.append(16-1, EXTRA_SIZE_BITS);
  packet.append(ADS[0], 16);
  
  // Fill any remaining bits (from rounding up to whole bytes) with 1's,
  // so they cannot be a valid field.
  packet.append(0xff, packet.free_bits());

  // Prepare upstream data transmission at the next possible time.
  LMIC_setTxData2(LORA_PORT, packet.data(), packet.byte_size(), 0);
  if (DEBUG)
  {
    Serial.println(F("que"));
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

void readLoadCell(){

  //Start I2C communicatrion
  
  Wire.begin();
  Wire.setClock(100000);
  Wire.setWireTimeout(WIRE_TIMEOUT);

  ads.setGain(GAIN_SIXTEEN);

  //check if device is found
  Wire.beginTransmission(0x048);
  int check=Wire.endTransmission();
  
  if(check==0){
    
    ADS[0]=ads.readADC_Differential_0_1()+20;
    
  }
  
  else if(check==5){ //timeout
    //Serial.println(F("Timeout"));
    ADS[0]=5;  
  }
  else{ //another error
    ADS[0]=0;  
  }

  if(DEBUG){
    Serial.println(F("ADC:"));
    Serial.println(ADS[0]);
  }

}
