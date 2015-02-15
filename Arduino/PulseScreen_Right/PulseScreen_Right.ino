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
   // UN-COMMENT THE NEXT LINE IF YOU ARE POWERING The Arduino at 5V and the Pulse Sensor AT 3V 
   // AND APPLY THE LOWER VOLTAGE TO THE A-REF PIN
//   analogReference(EXTERNAL);   
}



void loop(){
  Serial.println('loop'); 
  for(int s=0; s<2; s++){
    Math(s);
    sendDataToProcessing(s, 'F', Signal[s]);     // send Processing the raw Pulse Sensor data
    delay(2)
  }
  while(mySerial.available() <= 0);
  Serial.println(mySerial.read());
  
    delay(10);
  //delay(2);    //  take a break
}


void sendDataToProcessing(int sensor, char symbol, int data ){
  Serial.print(sensor);
  Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
  Serial.println(data);                // the data to send culminating in a carriage return
  }
