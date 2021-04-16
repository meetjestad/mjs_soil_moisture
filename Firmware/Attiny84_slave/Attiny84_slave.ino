// Code for the ATtiny84
// Board = ATtiny84
//Processor = Attiny84
//Clock speed = 8mhz
//Programmer = Arduino as ISP

// VERSION = Fex_V0 : for use on older PCBs

#include <SoftwareSerial.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define RX 4 //check SCL
#define TX 5 //check SDA

SoftwareSerial Vserial(RX,TX);

//fixed measurement parameters
const uint8_t samples=10;
const uint8_t num=6; //4 levels, 1x NC and 1x reference
const unsigned long interval = 100000; //us
const uint8_t R=1; //Mohm

//measurement variables
float C_raw_freq[samples];
uint8_t reg=0;
int t=0; 
uint8_t checksum=0;

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

uint16_t RF[num];
uint8_t T[num];

//pin declaration
int S0=7;
int S1=9;
int S2=10;
int NTC_L=1;
int NTC_H=0;
int NTC_I=A2;
int F_OUT=8;
int E=3; //remove for newer PCBs

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

  //start the measurement
  pulseCount = 0;
  lastReportTime=micros();
  pulseOn=true;

  while(c<samples){

    //check if interval has passed
    if (micros()-lastReportTime >= interval) {

      true_interval=micros() - lastReportTime;
      
      // record the amount of pulses (corrected for true interval
      pulseOn=false;
      C_raw_freq[c]=float(pulseCount)*1000000.0/float(true_interval);
      c++;

      //restart
      lastReportTime=micros();
      pulseCount = 0;
      pulseOn=true;
    }
  }

  //calcultae result
  pulseOn=false;

  RF[reg]=array_mean(C_raw_freq)*R; //frequency*R
  T[reg]=calc_temp(NTC_temp(10)); //10=number of samples

  //reset the array
  for(int i=0; i<samples;i++){
    C_raw_freq[i]=0;
  }
  
}

void setup(){

  //TX HIGH
  pinMode(TX, OUTPUT);
  digitalWrite(TX,HIGH);
  
  //Set multiplexer pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);

  //remove for newer boards
  pinMode(E, OUTPUT);
  digitalWrite(E, LOW);

  //set the multiplexer values
  setMult();

  //set NTC pins
  pinMode(NTC_L, INPUT); // floating
  pinMode(NTC_H, INPUT); // floating
  pinMode(NTC_I, INPUT); // actual input

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

  //------------------MEASURE--------------------
  
  //In setup, so that the loop is only attempting 
  //to send the data over TTL-UART
  
  for(int n=0;n<num;n++){

    reg=n;
    
    //set the multiplexer values
    digitalWrite(S0,L[reg].S0);
    digitalWrite(S1,L[reg].S1);
    digitalWrite(S2,L[reg].S2);

    //Switching delay
    delay(100);

    measure_loop();
  }

  delay(500); //time for F_OUT to stabalize
 
}

void loop(){

  //start serial connection
  Vserial.begin(9600);

  t=0;
  while(!Vserial && t<100){
    delay(100);
    t++;
  }
   
 if(t<=100){
  
   checksum=0;
   for(int n=0;n<num;n++){
     uint8_t* RF_byte= (uint8_t*)&RF[n];
     Vserial.write(RF_byte[0]);
     Vserial.write(RF_byte[1]);
     Vserial.write(T[n]);
     checksum+=RF_byte[0]+RF_byte[1]+T[n];
   }

   //send the measurement resolution and sample size
   Vserial.write(samples);
   checksum=checksum+samples;

   //send the checksum
   Vserial.write(checksum);
   delay(400);
}
else{
   for(int n=0;n<num;n++){
     Vserial.write(10);
     Vserial.write(10);
     Vserial.write(10);
     checksum=checksum+30;
   }

   //send the measurement resolution and sample size
   Vserial.write(samples);
   checksum=checksum+samples;

   //send the checksum
   Vserial.write(checksum);
   delay(400);
}
}

uint8_t calc_temp(uint16_t Tint){

  //ADC values
  float Vcc = 2.5;
  float steps = 1024;

  //NTC parameters
  float r0=100000;
  float T0=25+273;
  float r_ref=125000; //adjust
  float beta=4036;

  float Tv=(Vcc/steps)*Tint;
  float Tr=(Vcc-Tv)/(Tv/r_ref);
  float Tc=1/(log(Tr/r0)/beta+1/T0)-273;

  uint8_t Traw = (Tc+20)*4;

  return Traw;

}
