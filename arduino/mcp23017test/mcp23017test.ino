#define SPEED_1 2
#define SPEED_2 3
#define SPEED_3 10
#define SPEED_4 29


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
   pinMode(SPEED_2, OUTPUT);
   pinMode(SPEED_3, OUTPUT);
   pinMode(SPEED_4, OUTPUT);
  
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
 Wire.beginTransmission(0x24);
 Wire.write(0x00); // IODIRA register
 Wire.write(0x00); // set all of port A to outputs
 Wire.endTransmission();
Wire.beginTransmission(0x24);
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
Wire.write(0x13); // address port B  //top left motor (upside down one)
Wire.write(0x0E);     // value to send
Wire.endTransmission();

Wire.beginTransmission(0x20);
 Wire.write(0x12); // GPIOB
 Wire.write(0x70); // port B
 Wire.endTransmission();

 Wire.beginTransmission(0x24); //right side
Wire.write(0x13); // address port B  //top left motor (upside down one)
Wire.write(0x0E);     // value to send
Wire.endTransmission();

Wire.beginTransmission(0x24);
 Wire.write(0x12); // GPIOB
 Wire.write(0x70); // port B
 Wire.endTransmission();
 
 
 //binaryCount();
   digitalWrite(LED, HIGH); 
   analogWrite(SPEED_1, 255);
   analogWrite(SPEED_2, 255);
   analogWrite(SPEED_3, 255);
   analogWrite(SPEED_4, 255);
   delay(2000);
   Wire.beginTransmission(0x20);
    Wire.write(0x13); // address port B 
    Wire.write(0x00);     // value to send
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
 Wire.write(0x12); // GPIOB
 Wire.write(0x00); // port B
 Wire.endTransmission();
     Wire.beginTransmission(0x24);
 Wire.write(0x13); // GPIOB
 Wire.write(0x00); // port B
 Wire.endTransmission();
     Wire.beginTransmission(0x24);
 Wire.write(0x12); // GPIOB
 Wire.write(0x00); // port B
 Wire.endTransmission();
   digitalWrite(LED, LOW);
   delay(2000);}
