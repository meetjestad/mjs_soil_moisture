//Code for the master recieving data from the 
// MJS_soil_moisture prototype slave device

#include <Wire.h>

//bytes that are expected
const int byte_number = 4;
uint8_t rec_buf[byte_number];

const uint8_t levels=6;
uint16_t l[levels];
uint8_t t[levels];

union {
    uint8_t b[2];
    uint16_t i;
  } C;

uint8_t T;

float T_conv;
float C_conv;

int n=0;
int c;
int samples=10;


void setup() {
  //Begin serial
  Serial.begin(9600); // start serial for output
  Serial.println("Requesting measurements from I2C slave device");

  //Start wire connection
  Wire.begin(); // join i2c bus (SDA,SCL)
  Wire.setClock(100000);
}

void loop() {

  if(n==0){
    Serial.print("NC :");
  }
  else if(n==1){
    Serial.print("Ref:");
  }
  else{
    Serial.print("L"+String(n-1)+" :");
  }
  
  readSoilMoisture(n);
  T_conv=float(t[n])/4-20;
    
  if(l[n]!=0){
    Serial.print(String(l[n])+" femto-F | "+String(T_conv)+" C\n");
  }
  else{
    delay(1000);
  }

  n++;

  if(n==levels){
    n=0;
    Serial.println();
  }
}

void readSoilMoisture(int lev){

  //bytes that are expected
  const int byte_number = 4;
  uint8_t rec_buf[byte_number];

  union {
    uint8_t b[2];
    uint16_t i;
  } C;

  uint8_t T;

  //start I2C communication
  Wire.beginTransmission(0x04);
  Wire.write(lev);
  int check=Wire.endTransmission();
  
  if(check==0){
    //Serial.print(F("Ack"));
    //wait for sensor
    delay(10000);

    //request measurement result
    Wire.requestFrom(0x04, byte_number, true); // request 1 byte from slave device address 4

    int i = 0;
    while (Wire.available()) {
      rec_buf[i]=Wire.read();
      i++;
    }

    //assign bytes to values
    C.b[0]=rec_buf[1];
    C.b[1]=rec_buf[2];
    T=rec_buf[3];

    //checksum if the expected number of bytes are recieved
    if (i = byte_number) {
      Serial.print(" Ack | ");
      l[lev] = C.i;
      t[lev] = T;      
    }
    
    else {
      //invalid number of bytes recievd or no data
      Serial.print(" Error, unexpected number of bytes\n");
      l[lev] = 0;
      t[lev] = 0;
    }
  }
  
  else{
    Serial.println(" Error, no I2C connection\n");
    l[lev] = 0;
    t[lev] = 0;    
  }

}
