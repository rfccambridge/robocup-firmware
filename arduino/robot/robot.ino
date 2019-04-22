#include "Motion.h"
#include "XBEE.h"
#include "Motor.h"

#define LED 13

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, 
// int ccw_addr, int enable_addr, int speed_addr, 
// int encoder_a, int encoder_b)
Motor motorBR(0x20, 0x13, 14, 7, 5, 2, 27, 28);
Motor motorFR(0x20, 0x12, 112, 224, 4, 3, 27, 28);
Motor motorBL(0x24, 0x13, 14, 7, 5, 10, 27, 28);
Motor motorFL(0x24, 0x12, 112, 224, 4, 29, 27, 28);

// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
Motion motion(motorBR, motorFR, motorBL, motorFL);
XBEE xbee(5);

void setup() {
  // put your setup code here, to run once:  
  // xbee.setup();
  Serial.begin(9600);
  motorFR.setup();
  motorBR.setup();
  motorFL.setup();
  motorBL.setup();
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  int input[4];
//  xbee.read_line(input);
//  int id = input[0];
//  int* v = &input[1];

  char buf[256];
  xbee.read_raw(buf);
  Serial.println(buf);
  
  motorBL.turn(50);  

}
