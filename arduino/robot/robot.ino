#include "Motion.h"
#include "XBEE.h"
#include "MotorEncoder.h"
#include "Motor.h"
#include "Dribbler.h"
#include "TimerOne.h"

#define LED 13
#define DEBUG_REPORT_TIME 500

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, 
// int ccw_addr, int enable_addr, int speed_addr, 
// int encoder_a, int encoder_b)
Motor motor_br(0x20, 0x13, 14,    7, 2, 16, 17);
Motor motor_fr(0x20, 0x12, 112, 224, 3, 28, 27);
Motor motor_bl(0x24, 0x12, 112, 224, 10, 14, 15);
Motor motor_fl(0x24, 0x13, 14,    7, 29, 26, 25);
Dribbler dribbler(6);

// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
IntervalTimer PIDUpdateTimer;
Motion motion(motor_br, motor_fr, motor_bl, motor_fl);
XBEE xbee(5);

#define IS_DEBUG true

// Used to time how fast we send debug messages back through the xbee
unsigned long last_debug_timer = 0;

Command latestCommand;

void update_encoder_br_a() { motor_br.encoder.update_a(); }
void update_encoder_br_b() { motor_br.encoder.update_b(); }
void update_encoder_fr_a() { motor_fr.encoder.update_a(); }
void update_encoder_fr_b() { motor_fr.encoder.update_b(); }
void update_encoder_bl_a() { motor_bl.encoder.update_a(); }
void update_encoder_bl_b() { motor_bl.encoder.update_b(); }
void update_encoder_fl_a() { motor_fl.encoder.update_a(); }
void update_encoder_fl_b() { motor_fl.encoder.update_b(); }


void debug_move_command(double x, double y, double w) {
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(x);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(w);
  Serial.print(" ");
  Serial.println("");
}


void setup() {  
  xbee.setup();
  motor_br.setup();
  motor_fr.setup();
  motor_bl.setup();
  motor_fl.setup();
  
  attachInterrupt(motor_br.encoder.encoder_a, update_encoder_br_a, CHANGE);
  attachInterrupt(motor_br.encoder.encoder_b, update_encoder_br_b, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_a, update_encoder_fr_a, CHANGE);
  attachInterrupt(motor_fr.encoder.encoder_b, update_encoder_fr_b, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_a, update_encoder_bl_a, CHANGE);
  attachInterrupt(motor_bl.encoder.encoder_b, update_encoder_bl_b, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_a, update_encoder_fl_a, CHANGE);
  attachInterrupt(motor_fl.encoder.encoder_b, update_encoder_fl_b, CHANGE);

  // initialize PID constants
  motion.setup(1.00, 0.0, 0); // [useable?] p-only tuning (with clearing integral windup?)
  // i-tuning we've tried - around 30 is pretty high?
  
  // use timer interrupts to make sure PID movement is being updated consistently
  // (interrupt rate must match with PID update loop rate for correct calculations)
  PIDUpdateTimer.begin(movePIDCallback, 1000000 / PID_UPDATE_HZ);
  // IMPORTANT - set interrupt to lowest priority, so encoder interrupts are not missed
  PIDUpdateTimer.priority(255);

  
  // according to docs, baud rate is ignored
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  memset((void*) &latestCommand, 0, sizeof(Command));
}

void movePIDCallback() {
  motion.update_PID();
}


void loop() {
  digitalWrite(LED, HIGH);
  
//  delay(100);
//  digitalWrite(LED, LOW);
//  delay(100);
//  motion.move_raw(0, 500, 0);
//  delay(1000);
//  motion.move_raw(0, 0, 0);
//  delay(1000000000);
  // motion.XYW_to_setpoints(380, 320, 0);

  if (millis() - last_debug_timer > DEBUG_REPORT_TIME) {
    last_debug_timer = millis();
    // Serial.println(motion.PID_report());
    // xbee.write_string(motion.PID_report());
  }
  // Read latest command from xbee into global buffer

  if (xbee.read_command(&latestCommand)) {
    // convert movement command into motor speed setpoints
    // (timer interrupt will trigger the actual PID updates)
    double x = latestCommand.vx;
    double y = latestCommand.vy;
    double w = latestCommand.vw;
    if (IS_DEBUG) {
       debug_move_command(x, y, w);
    }
    motion.XYW_to_setpoints(x, y, w);
  
    if (latestCommand.is_dribbling) {
      dribbler.spin(255);
    } else {
      dribbler.spin(0);
    }
  }
  // Delay loop (avoiding using delay() function)
  int delay_time = 10;
  int start_time, end_time;
  start_time = end_time = millis();
  while(end_time - start_time < delay_time) {
    end_time = millis();
  }
  // Serial.println("done loop");
  // Serial.println(millis());
}
