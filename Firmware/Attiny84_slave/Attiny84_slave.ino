// Code for the ATtiny84
// Board = ATtiny84
//Processor = Attiny84
//Clock speed = 8mhz
//Programmer = Arduino as ISP

// VERSION = V2.3

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave
 
#include <TinyWireS.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//measurement 
const int samples=50;
const int num=6; //4 levels, 1x NC and 1x reference
unsigned long interval = 10000;
float C_raw_freq[samples];
float k = 1;
float R=1000000; //Mohm
uint8_t Vcc;

//for the I2C data transfer
uint8_t reg=0;

//for interrupt
volatile long pulseCount; 
unsigned long lastReportTime;
unsigned long true_interval;
volatile boolean pulseOn; 

//Data storage and settings for each level
struct level{
  int S0;
  int S1;
  int S2;
};

struct level L[num];

union{
  uint16_t i;
  unsigned char b[2];
} C;

uint8_t T;

//pin declaration
int S0=7;
int S1=9;
int S2=10;
int NTC_L=1;
int NTC_H=0;
int NTC_I=A2;
int VCC=A3;
int F_OUT=8;

//multiplexer settings
void setMult(){
  L[0].S0=0; // A6 = 2 = Not connected
  L[0].S1=1;
  L[0].S2=1;
  
  L[1].S0=0; // A4 = 1 = Reference
  L[1].S1=0;
  L[1].S2=1;
  
  L[2].S0=0; // A2 = 15 = L1
  L[2].S1=1;
  L[2].S2=0;
  
  L[3].S0=1; // A1 = 14 = L2
  L[3].S1=0;
  L[3].S2=0;

  L[4].S0=0; // A0 = 13 = L3
  L[4].S1=0;
  L[4].S2=0;

  L[5].S0=1;  //A3 = 12 = L4
  L[5].S1=1;
  L[5].S2=0;

}

//interrupt function
ISR(INT0_vect){
  if(pulseOn){
    pulseCount++;
  }
} 

//averaging function
float array_mean(float arr[samples]){
  float sum=0;
  float mean;
  for(int i=0;i<samples;i++){
    sum+=arr[i];
  }
  
  mean=sum/float(samples);
  return mean;
}

//standard deviation
float array_stdev(float arr[samples]){
  float mean=array_mean(arr);
  float var=0;
  float st;
  
  for(int i=0;i<samples;i++){
    var+=pow((float(arr[i])-mean),2);
  }
  
  st=pow((var/float(samples)),0.5);
  return st;
}

uint16_t NTC_temp(int tsample){

  //set to output
  pinMode(NTC_L, OUTPUT); 
  pinMode(NTC_H, OUTPUT); 
  delay(100);  
  digitalWrite(NTC_L, LOW);
  digitalWrite(NTC_H, HIGH);
  delay(100);

  //measurement
  int T_sum=0;
  int t=0;
  while(t<tsample){
    T_sum+=analogRead(NTC_I);
    t=t+1; 
  }

  pinMode(NTC_L, INPUT); // floating
  pinMode(NTC_H, INPUT); // floating

  return T_sum/t;  
  
}

//measurement loop
void measure_loop(){
  int c=0;
  
  //set the multiplexer
  digitalWrite(S0,L[reg].S0);
  digitalWrite(S1,L[reg].S1);
  digitalWrite(S2,L[reg].S2);

  //Switching delay
  delay(100);

  //start the measurement
  pulseCount = 0;
  lastReportTime=micros();
  pulseOn=true;

  while(c<samples){

    //check if interval has passed
    if (micros()-lastReportTime >= interval) {

      true_interval=micros() - lastReportTime;
      
      // record the amount of pulses
      pulseOn=false;
      C_raw_freq[c]=1000000*pulseCount/true_interval;
      c++;

      //restart
      lastReportTime=micros();
      pulseCount = 0;
      pulseOn=true;
    }
  }

  //calcultae result
  pulseOn=false;

  //calculate and store frequency data 
  C.i=pow(10,15)/(array_mean(C_raw_freq)*R);
  T=calc_temp(NTC_temp(10)); //10=number of samples

  //reset the array
  for(int i=0; i<samples;i++){
    C_raw_freq[i]=0;
  }

  //calculate the sensor voltage
  uint16_t Vcc_send=VCC_read(10)/4;
  Vcc=Vcc_send;

  //multiplexer disabled
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,HIGH);
}

void setup(){

  //Set the pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  pinMode(NTC_L, INPUT); // floating
  pinMode(NTC_H, INPUT); // floating
  pinMode(NTC_I, INPUT); // actual input
  pinMode(VCC, INPUT); // actual input

  //multiplexer disabled
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,HIGH);

  //set the multiplexer values
  setMult();
    
  // start I2C
  TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
  TinyWireS.onRequest(requestEvent);
  TinyWireS.onReceive(recieveEvent);

  //do not start counting pulses yet
  pulseOn=false;


  //attach interrupt //Taken from https://forum.arduino.cc/index.php?topic=427955.0
  // Interrupt on INT0 working, on PCIE1 interferes with SDA and SCL
  PCMSK1 |= (1 << PCINT10); // pin 8 or PCINT10 as poin change interrupt
  //GIFR |= (1 << PCIF1);  //clear any outstanding interrupts   
  GIMSK |= (1 << INT0); // enable external pin change interrupt
  MCUCR |= (1<<ISC01)|(1<<ISC00); // set fire condition: rising edge
  pinMode(8, INPUT_PULLUP); //stabalize the pin
  sei();
 
}

void loop(){
    // This needs to be here
    TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent(){
    
    //send the register
    TinyWireS.send(reg);
    TinyWireS.send(C.b[0]);
    TinyWireS.send(C.b[1]);

    //for NC send the value of VCC / 4
    if(reg!=0){
      TinyWireS.send(T);
    }
    else{
      TinyWireS.send(Vcc);
    }
}

void recieveEvent(){
  
  while(TinyWireS.available()){
    reg=TinyWireS.receive();
  }

  //perform measurement for this level
  measure_loop();
  
}

uint8_t calc_temp(uint16_t Tint){

  //VCC calue
  uint16_t VCCint = VCC_read(10);

  //NTC parameters
  float r0=100000;
  float T0=25+273;
  float r_ref=125000; //adjust
  float beta=4036;

  float Tv=float(Tint)/float(VCCint);
  float Tr=r_ref*(1-Tv)/Tv;
  float Tc=1/(log(Tr/r0)/beta+1/T0)-273;

  uint8_t Traw = (Tc+20)*4;

  return Traw;

}

uint16_t VCC_read(int tsample){
  
  //measurement
  uint16_t VCC_sum=0;
  uint16_t t=0;
  
  while(t<tsample){
    VCC_sum+=analogRead(VCC);
    t=t+1; 
  }

  return VCC_sum/t;  
}
