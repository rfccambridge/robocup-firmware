#include "Motion.h"
#include "XBEE.h"
#include "MotorEncoder.h"
#include "Motor.h"
#include "Dribbler.h"
#include "TimerOne.h"

#define LED 13
#define ROBOT_ID 8 // Change per robot

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, 
// int ccw_addr, int enable_addr, int speed_addr, 
// int encoder_a, int encoder_b)
Motor motor_br(0x20, 0x13, 14,    7, 2, 16, 17);
Motor motor_fr(0x20, 0x12, 112, 224, 3, 28, 27);
Motor motor_bl(0x24, 0x12, 112, 224, 10, 14, 15);
Motor motor_fl(0x24, 0x13, 14,    7, 29, 26, 25);
Dribbler dribbler(6);


// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
Motion motion(motor_br, motor_fr, motor_bl, motor_fl);
XBEE xbee(5);

double latestCommand[5] = { 0 };
double latestMoveXYW[3] = { 0 };
// TODO: store timestamp, so we can expire after .2 seconds or so

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

  // initialize pid (k_p, k_i, k_d)
  motion.setup(90.0, 0.0, 0.0);
  
  attachInterrupt(motor_br.encoder.encoder_a, update_encoder_br_a, CHANGE);
  attachInterrupt(motor_br.encoder.encoder_b, update_encoder_br_b, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_a, update_encoder_fr_a, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_b, update_encoder_fr_b, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_a, update_encoder_bl_a, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_b, update_encoder_bl_b, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_a, update_encoder_fl_a, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_b, update_encoder_fl_b, CHANGE);

  // use timer interrupts to make sure PID movement is being updated consistently
  Timer1.initialize(2000); // WHY things start getting weird below 3ms
  Timer1.attachInterrupt(movePIDCallback);
  
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void movePIDCallback() {
  /* Serial.print(latestMoveXYW[0]);
  Serial.print(latestMoveXYW[1]);
  Serial.println(latestMoveXYW[2]); */
  // motion.move(0, 5, 0); 
  motion.move(latestMoveXYW[0], latestMoveXYW[1], latestMoveXYW[2]); 
}

void loop() {
  digitalWrite(LED, HIGH);
//  // Serial.print("test");
//  delay(100);
//  digitalWrite(LED, LOW);
//  delay(100);
  // motion.move_raw(0, 0, 70);
    
  // Read latest command from xbee into global variable "input"
  xbee.read_line(latestCommand);
  int robot_id = latestCommand[0];
  int cmd = (int) latestCommand[1];
  if (robot_id == -1 || robot_id == ROBOT_ID) {
    if (cmd == CMD_MOVE) {
      // save movement commands, let the timer interrupt execute it
      // RIGHT NOW ITS RECEIVING INTERMITTENT 0's FROM GAMECONTROLLER SOMEHOW 
      latestMoveXYW[0] = latestCommand[2];
      latestMoveXYW[1] = latestCommand[3];
      latestMoveXYW[2] = latestCommand[4];
    }
    else if (cmd == CMD_DRIBBLE) {
      dribbler.spin(latestCommand[2]);
    }
    else if (cmd == CMD_KILL) {
      dribbler.spin(0);
      motion.stop();
      while(true);
    }
    else {
      // Serial.println("Unrecognized command");
    }
  }

  // Serial.println(millis());
  // Serial.flush();
  int delay_time = 1;
  int start_time, end_time;
  start_time = end_time = millis();
  while(end_time - start_time < delay_time) {
    end_time = millis();
  }
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
