#include "Motion.h"
#include "XBEE.h"
#include "Motor.h"

#define LED 13

Motor motor(0x20, 0x13, 0x4, 0x3, 0x5, 0xA);
XBEE xbee(5);

void setup() {
  // put your setup code here, to run once:  
//  xbee.s/etup();
Serial.begin(9600);
motor.setup();
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int input[4];
//  xbee.read_line(input);
//  int id = input[0];
//  int* v = &input[1];

  digitalWrite(LED, HIGH);
  motor.turn(255);
  delay(2000);
  digitalWrite(LED, LOW);
  motor.turn(-1000);
  delay(2000);
}
