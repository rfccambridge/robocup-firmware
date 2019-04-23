#include "Motion.h"
#include "XBEE.h"
#include "Motor.h"

#define LED 13

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, 
// int ccw_addr, int enable_addr, int speed_addr, 
// int encoder_a, int encoder_b)
Motor motorBR(0x20, 0x13, 14,    7, 5,  2, 27, 28);
Motor motorFR(0x20, 0x12, 112, 224, 4,  3, 27, 28);
Motor motorBL(0x24, 0x12, 112, 224, 5, 10, 27, 28);
Motor motorFL(0x24, 0x13, 14,    7, 4, 29, 27, 28);

// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
Motion motion(motorBR, motorFR, motorBL, motorFL);
XBEE xbee(5);

void setup() {
  // put your setup code here, to run once:  
  xbee.setup();
  Serial.begin(9600);
  motorFR.setup();
  motorBR.setup();
  motorFL.setup();
  motorBL.setup();
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  /* Test if things are clockwise or counter */
//  motorBR.turn(50);
//  delay(500);
//  motorBR.turn(0);
//  
//   motorFR.turn(55);
//  delay(500);
//  motorFR.turn(0);
//  
//  motorBL.turn(50);
//  delay(500);
//  motorBL.turn(0);
//  motorFL.turn(50); 
//  delay(500);
//  motorFL.turn(0); 

//  motorBR.turn(50);
//  motorFR.turn(50);
//  motorBL.turn(50);
//  motorFL.turn(50);
//  char buf[256];
  int input[4];
  xbee.read_line(input);
  Serial.print(input[0]);
  Serial.print(",");
  Serial.print(input[1]);
  Serial.print(",");
  Serial.print(input[2]);
  Serial.print(",");
  Serial.print(input[3]);
  Serial.println("");
  motion.move(input[1], input[2], input[3]);
  delay(30);
  /*
  digitalWrite(LED, HIGH);
  motorBR.turn(50);
  motorFR.turn(50);
  motorBL.turn(50);
  motorFL.turn(50);
  delay(1000);
  digitalWrite(LED, LOW);
  motorBR.turn(-50);
  motorFR.turn(-50);
  motorBL.turn(-50);
  motorFL.turn(-50);
  delay(1000);
  */
}
