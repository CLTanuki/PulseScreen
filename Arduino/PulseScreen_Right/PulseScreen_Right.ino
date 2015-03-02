#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 10); // RX, TX
//  VARIABLES
int pulsePin[3] = {A0, A1, A2};                 // Pulse Sensor purple wire connected to analog pin 0

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM[3];                   // used to hold the pulse rate
volatile int Signal[3];                // holds the incoming raw data
volatile int IBI[3] = {600, 600, 600};             // holds the time between beats, must be seeded! 
volatile boolean Pulse[3] = {false, false, false};     // true when pulse wave is high, false when it's low
volatile boolean QS[3] = {false, false, false};        // becomes true when Arduoino finds a beat.
byte data[6][4];



void setup(){
  Serial.begin(115200);             // we agree to talk fast!
  mySerial.begin(9600);
  delay(4);  
}

byte Sensors[6][2];

void loop()
{
  ReadData();
  for(int i=0; i < 3; i++)
  {
    Math(i);
    delay(1);
  }
  sendDataToProcessing();
}


void sendDataToProcessing()
{  
  for(int i = 0; i < 3; i++)
  {
    Sensors[i+3][1] = Signal[i];
    Sensors[i+3][0] = Signal[i]>>8;
//    if(QS[i])  Sensors[i+3][0] |= 0b10000000;
  }
  
  Serial.write(0xFF);
  Serial.write(0xFF);

  for(int i = 0; i < 6; i++)
  {
    Serial.write(Sensors[i][0]);
    Serial.write(Sensors[i][1]);
  }
  digitalWrite(13, LOW);
}
  
void ReadData()
{
  while(mySerial.available() <= 0);
  if(mySerial.read() != 0xFF)  return;

  while(mySerial.available() <= 0);
  if(mySerial.read() != 0xFF)  return;

  while(mySerial.available() <= 0);
  Sensors[0][0] = mySerial.read();

  while(mySerial.available() <= 0);
  Sensors[0][1] = mySerial.read();

  while(mySerial.available() <= 0);
  Sensors[1][0] = mySerial.read();

  while(mySerial.available() <= 0);
  Sensors[1][1] = mySerial.read();

  while(mySerial.available() <= 0);
  Sensors[2][0] = mySerial.read();

  while(mySerial.available() <= 0);
  Sensors[2][1] = mySerial.read();
}
