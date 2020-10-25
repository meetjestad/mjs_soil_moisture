 //before uploading set the arduino as ISP, using the example sketch, ArduinoISP

// Code for the ATtiny84
// Board = ATtiny84
//Processor = Attiny84
//Clock speed = 8mhz
//Programmer = Arduino as ISP

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave
 
#include <TinyWireS.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//measurement 
const int samples=50;
const int num=6; //4 levels and 2xreference
unsigned long interval = 10000;
float C_raw_freq[samples];
float k = 1;
float R=996000; //Mohm

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
  int E;
};

struct level L[num];

union fl{
  float F;
  unsigned char b[4];
};

fl av;
fl st;

union t{
  uint16_t raw;
  unsigned char b[2];
} T;

//pin declaration
int S0=7;
int S1=9;
int S2=10;
int E=3;
int NTC_L=1;
int NTC_H=0;
int NTC_I=A2;
int F_OUT=8;

//multiplexer settings
void setMult(){
  L[0].S0=0;
  L[0].S1=0;
  L[0].S2=0;
  L[0].E=1;
  
  L[1].S0=0;
  L[1].S1=1;
  L[1].S2=0;
  L[1].E=0;
  
  L[2].S0=1;
  L[2].S1=0;
  L[2].S2=0;
  L[2].E=0;
  
  L[3].S0=0;
  L[3].S1=0;
  L[3].S2=0;
  L[3].E=0;
  
  L[4].S0=1;
  L[4].S1=1;
  L[4].S2=0;
  L[4].E=0;

  L[5].S0=0;
  L[5].S1=0;
  L[5].S2=1;
  L[5].E=0;
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
  digitalWrite(E,L[reg].E);

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
  av.F=array_mean(C_raw_freq);
  st.F=array_stdev(C_raw_freq);
  T.raw=NTC_temp(10); //10=number of samples

  //reset the array
  for(int i=0; i<samples;i++){
    C_raw_freq[i]=0;
  }
}

void setup(){

  //Set the pins
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(E, OUTPUT);

  pinMode(NTC_L, INPUT); // floating
  pinMode(NTC_H, INPUT); // floating
  pinMode(NTC_I, INPUT); // actual input

  digitalWrite(S0,LOW);
  digitalWrite(S1,LOW);
  digitalWrite(S2,LOW);
  digitalWrite(E,LOW);

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
    TinyWireS.send(av.b[0]);
    TinyWireS.send(av.b[1]);
    TinyWireS.send(av.b[2]);
    TinyWireS.send(av.b[3]);
    TinyWireS.send(st.b[0]);
    TinyWireS.send(st.b[1]);
    TinyWireS.send(st.b[2]);
    TinyWireS.send(st.b[3]);
    TinyWireS.send(T.b[0]);
    TinyWireS.send(T.b[1]);
    
}

void recieveEvent(){
  
  while(TinyWireS.available()){
    reg=TinyWireS.receive();
  }

  //perform measurement for this level
  measure_loop();
  
}
