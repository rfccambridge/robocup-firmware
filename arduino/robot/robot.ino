#include "Motion.h"
#include "XBEE.h"
#include "Encoder.h"
#include "Motor.h"

#define LED 13

#define RH_ENCODER_A 14
#define RH_ENCODER_B 15

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

int input[4];

volatile unsigned long rightCount = 0;

void rightEncoderEvent() {
  Serial.println("FUCK YOU");
  Serial.println(digitalRead(RH_ENCODER_A));
  Serial.println(digitalRead(RH_ENCODER_B));
  rightCount += (digitalRead(RH_ENCODER_A) == digitalRead(RH_ENCODER_B)) ? -1 : 1;
//  if (digitalRead(RH_ENCODER_A) == HIGH) {
//    if (digitalRead(RH_ENCODER_B) == LOW) {
//      rightCount++;
//    } else {
//      rightCount--;
//    }
//  } else {
//    if (digitalRead(RH_ENCODER_B) == LOW) {
//      rightCount--;
//    } else {
//      rightCount++;
//    }
//  }
}

void setup() {
  pinMode(RH_ENCODER_A, INPUT);
  pinMode(RH_ENCODER_B, INPUT);
  attachInterrupt(1, rightEncoderEvent, CHANGE);
  
  // put your setup code here, to run once:  
  xbee.setup();
  motorBR.setup();
  motorFR.setup();
  motorBL.setup();
  motorFL.setup();
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  for (int i = 0; i < 4; i++) {
    input[i] = 0;
  }

  
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
  xbee.read_line(input);
 
  Serial.println(motorBR.position());
  Serial.println(motorFR.position());
  Serial.print("Right Count: ");
  Serial.println(rightCount);
  Serial.println();
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
  // put your main code here, to run repeatedly:
//  int input[4];
//  xbee.read_line(input);
//  int id = input[0];
//  int* v = &input[1];

//  char buf[256];
//  xbee.read_raw(buf);
//  Serial.println(buf);
//  
//  motorBL.turn(50);  

}
