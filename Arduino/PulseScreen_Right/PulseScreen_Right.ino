#include <SoftwareSerial.h>

SoftwareSerial mySerial(A5, A4); // RX, TX
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
  for(int s=0; s<=2; s++)
  {
    Math(s);
    sendDataToProcessing(s, QS[s], Signal[s]);
    QS[s] = false;
    delay(2);
  }
  ReadData();
  delay(2);
}


void sendDataToProcessing(int sensor, bool qs, int data ){
  Serial.write(sensor);
  Serial.write(qs);                // symbol prefix tells Processing what type of data is coming
  Serial.write(data);                // the data to send culminating in a carriage return
  Serial.write(data>>8);
  }
  
void ReadData()
{
  int C1, C2, C3, C4;
  while(mySerial.available() <= 0);
  C1 = mySerial.read();
  while(mySerial.available() <= 0);
  C2 = mySerial.read();
  while(mySerial.available() <= 0);
  C3 = mySerial.read();
  while(mySerial.available() <= 0);
  C4 = mySerial.read();

  int Data = C3 + C4<<8;
  Serial.write(C1);
  Serial.write(C2);
  Serial.write(Data);
}
