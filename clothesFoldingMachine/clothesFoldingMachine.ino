#include <SoftwareSerial.h>

#define ledPin 13
SoftwareSerial blueSerial (2,3);//rx,tx 
int state = 0;

class BlueHandler{
  
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  blueSerial.begin(9600); // Default communication rate of the Bluetooth module
}
void loop() {
  if(blueSerial.available() > 0){ // Checks whether data is comming from the serial port
    state = blueSerial.read(); // Reads the data from the serial port
    Serial.print("leitura concluida:");
    Serial.println(state);
 }
 if (state == '0') {
  digitalWrite(ledPin, LOW); // Turn LED OFF
  blueSerial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
  state = 0;
  Serial.println("led: off");
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  blueSerial.println("LED: ON");;
  state = 0;
  Serial.println("led: on");
 } 
}
