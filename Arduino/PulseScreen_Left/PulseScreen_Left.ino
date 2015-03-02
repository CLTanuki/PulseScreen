#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
//  VARIABLES
int pulsePin[3] = {A0, A1, A2};                 // Pulse Sensor purple wire connected to analog pin 0

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM[3];                   // used to hold the pulse rate
volatile int Signal[3];                // holds the incoming raw data
volatile int IBI[3] = {600, 600, 600};             // holds the time between beats, must be seeded! 
volatile boolean Pulse[3] = {false, false, false};     // true when pulse wave is high, false when it's low
volatile boolean QS[3] = {false, false, false};        // becomes true when Arduoino finds a beat.


void setup(){
  Serial.begin(115200);             // we agree to talk fast!
  mySerial.begin(9600);
}


void loop()
{ 
  for(int i=0; i < 3; i++)
  {
    Math(i);
    delay(1);
  }
  sendDataToProcessing();
}


void sendDataToProcessing()
{
  byte SH[3], SL[3];
  for(int i = 0; i < 3; i++)
  {
    SL[i] = Signal[i];
    SH[i] = Signal[i]>>8;
//    if(QS[i])  SH[i] |= 0b10000000;
  }
  
  mySerial.write(0xFF);
  mySerial.write(0xFF);

  for(int i = 0; i < 3; i++)
  {
    mySerial.write(SH[i]);
    mySerial.write(SL[i]);    
  }
}
