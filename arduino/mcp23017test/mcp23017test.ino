#define SPEED_1 10

#define LED 13
/*
 Example 41.1 - Microchip MCP23017 with Arduino
 http://tronixstuff.com/tutorials > chapter 41
 John Boxall | CC by-sa-nc
*/
// pins 15~17 to GND, I2C bus address is 0x20
#include "Wire.h"
void setup()
{
   pinMode(SPEED_1, OUTPUT);
  pinMode(LED,OUTPUT);
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
}
void binaryCount()
{
 for (byte a=0; a<256; a++)
 {
 Wire.beginTransmission(0x20);
 Wire.write(0x12); // GPIOA
 Wire.write(a); // port A
 Wire.endTransmission();
Wire.beginTransmission(0x20);
 Wire.write(0x13); // GPIOB
 Wire.write(a); // port B
 Wire.endTransmission();
 }
}
void loop()
{

Wire.beginTransmission(0x20);
Wire.write(0x13); // address port B 
Wire.write(0x18);     // value to send
Wire.endTransmission();
 //binaryCount();
   digitalWrite(LED, HIGH); 
   analogWrite(SPEED_1, 40);
   delay(2000);
   Wire.beginTransmission(0x20);
    Wire.write(0x13); // address port B 
    Wire.write(0x14);     // value to send
    Wire.endTransmission();
   digitalWrite(LED, LOW);
   delay(2000);}
