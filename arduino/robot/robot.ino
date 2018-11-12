#include "Motion.h"
#include "XBEE.h"
#include "Motor.h"

#define LED 13

Motor motor1(0x20, 0x13, 4, 3, 5, 10, 27, 28);
Motor motor2(0x20, 0x12, 6, 5, 4, 29, 27, 28);
XBEE xbee(5);

void setup() {
  // put your setup code here, to run once:  
  // xbee.setup();
  Serial.begin(9600);
  motor1.setup();
  motor2.setup();
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int input[4];
//  xbee.read_line(input);
//  int id = input[0];
//  int* v = &input[1];

  digitalWrite(LED, HIGH);
  motor1.turn(50);
  motor2.turn(50);
  delay(2000);
  digitalWrite(LED, LOW);
  motor1.turn(-50);
  motor2.turn(-50);
  delay(2000);
}
