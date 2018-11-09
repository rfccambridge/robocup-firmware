#include "Wire.h"

#define CHARGE 0
#define CHARGE_STOP LOW
#define CHARGE_START HIGH

#define DISCHARGE 4
#define DISCHARGE_ON HIGH
#define DISCHARGE_OFF LOW

#define CHIPENABLE 5
#define CHIPENABLE_ON HIGH
#define CHIPENABLE_OFF LOW

#define DRIBBLER 6
#define DRIBBLER_ON HIGH
#define DRIBBLER_OFF LOW 

#define KICKENABLE 7
#define KICKENABLE_ON HIGH
#define KICKENABLE_OFF LOW

#define INPUT_A_1 3
#define INPUT_B_1 4
#define ENABLE_1 5
#define SPEED_1 10

#define LED 13

/*
 * XBEEs are connected on the PCB to Teensys on Serial 5
 * The connection to the computer is Serial 1
 */

void setup() {
  Serial5.begin(9600);
  Serial.begin(9600);
 
  pinMode(LED, OUTPUT);
  pinMode(CHARGE, OUTPUT);

  pinMode(DISCHARGE, OUTPUT);
  pinMode(CHIPENABLE, OUTPUT);
  pinMode(DRIBBLER, OUTPUT);
  pinMode(KICKENABLE, OUTPUT);  


  
  Wire.begin(); // wake up I2C bus
// set I/O pins to outputs
 Wire.beginTransmission(0x20);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();
Wire.beginTransmission(0x20);
 Wire.write(0x01); // IODIRB register
 Wire.write(0x00); // set all of port B to outputs
 Wire.endTransmission();
  
  pinMode(SPEED_1, OUTPUT);

}

void loop() {
  // Disable charging and kick
   digitalWrite(CHARGE, CHARGE_STOP);
   digitalWrite(KICKENABLE, KICKENABLE_OFF);
   digitalWrite(DISCHARGE, DISCHARGE_OFF);
   digitalWrite(CHIPENABLE, CHIPENABLE_OFF);
   digitalWrite(DRIBBLER, DRIBBLER_OFF);

  
   
   digitalWrite(LED, HIGH); 
   Wire.beginTransmission(0x20);
Wire.write(0x13); // address port B 
Wire.write(0x18);     // value to send
Wire.endTransmission();
   digitalWrite(LED, HIGH); 
   analogWrite(SPEED_1, 40);
   delay(2000);
   Wire.beginTransmission(0x20);
    Wire.write(0x13); // address port B 
    Wire.write(0x14);     // value to send
    Wire.endTransmission();
   digitalWrite(LED, LOW);
   delay(2000);
  
  
  if (Serial5.available())
  { // If data comes in from XBee, send it out to serial monitor
    String receivedStr = Serial5.readStringUntil('\n');
    Serial.println(receivedStr);
  }
  

}
