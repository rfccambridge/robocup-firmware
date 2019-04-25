#include "Motion.h"
#include "XBEE.h"
#include "MotorEncoder.h"
#include "Motor.h"

#define LED 13

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, 
// int ccw_addr, int enable_addr, int speed_addr, 
// int encoder_a, int encoder_b)
Motor motor_br(0x20, 0x13, 14,    7, 5,  2, 16, 17);
Motor motor_fr(0x20, 0x12, 112, 224, 4,  3, 27, 28);
Motor motor_bl(0x24, 0x12, 112, 224, 5, 10, 14, 15);
Motor motor_fl(0x24, 0x13, 14,    7, 4, 29, 25, 26);



// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
Motion motion(motor_br, motor_fr, motor_bl, motor_fl);
XBEE xbee(5);

int input[4];

void update_encoder_br_a() { motor_br.encoder.update_a(); }
void update_encoder_br_b() { motor_br.encoder.update_b(); }
void update_encoder_fr_a() { motor_fr.encoder.update_a(); }
void update_encoder_fr_b() { motor_fr.encoder.update_b(); }
void update_encoder_bl_a() { motor_bl.encoder.update_a(); }
void update_encoder_bl_b() { motor_bl.encoder.update_b(); }
void update_encoder_fl_a() { motor_fl.encoder.update_a(); }
void update_encoder_fl_b() { motor_fl.encoder.update_b(); }

void setup() {  
  xbee.setup();
  motor_br.setup();
  motor_fr.setup();
  motor_bl.setup();
  motor_fl.setup();

  // initialize pid
  motion.setup(0.0, 0.0, 5.0);
  
  attachInterrupt(motor_br.encoder.encoder_a, update_encoder_br_a, CHANGE);
  attachInterrupt(motor_br.encoder.encoder_b, update_encoder_br_b, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_a, update_encoder_fr_a, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_b, update_encoder_fr_b, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_a, update_encoder_bl_a, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_b, update_encoder_bl_b, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_a, update_encoder_fl_a, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_b, update_encoder_fl_b, CHANGE);
  
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  for (int i = 0; i < 4; i++) {
    input[i] = 0;
  }
  
}

void loop() {
  digitalWrite(LED, HIGH);
  xbee.read_line(input);
  motion.move(input[1], input[2], input[3]);
  // need a tiny delay or the motion will think NaN speed lol
  delay(10);
}

/* Some test code for initial encoder */
//  xbee.read_line(input);
//  Serial.println(motor_br.position());
//  Serial.println(motor_fr.position());
//  Serial.println(motor_bl.position());
//  Serial.println(motor_fl.position());
//  Serial.println();
//  Serial.print(input[0]);
//  Serial.print(",");
//  Serial.print(input[1]);
//  Serial.print(",");
//  Serial.print(input[2]);
//  Serial.print(",");
//  Serial.print(input[3]);
//  Serial.println("");
//  motion.move(input[1], input[2], input[3]);
//  motor_br.reset_position();
//  motor_fr.reset_position();
//  motor_bl.reset_position();
//  motor_fl.reset_position();
//  delay(100);


/* Some test code for initial PID */
//  Serial.print("PID Output: ");
//  Serial.print(pid_out);
//  motor_br.turn(pid_out);
//  int ticks = motor_br.position();
//  double tps = (double) ticks;
//  pid_in = tps;
//  Serial.print("  Actual TPS: ");
//  Serial.println(tps);
//  motor_br.reset_position();
//  myPID.Compute();
//  delay(50);
