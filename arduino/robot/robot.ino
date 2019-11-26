#include "Motion.h"
#include "XBEE.h"
#include "MotorEncoder.h"
#include "Motor.h"
#include "Dribbler.h"
#include "TimerOne.h"
#include "Kicker.h"

// must be true for robot to move, turning off helps printing?
#define IS_PID_INTERRUPT true
#define LED 13
#define IS_DEBUG_REPORT false
#define DEBUG_REPORT_TIME 500

// Motor::Motor(int mcp_addr, int motor_addr, int cw_addr,
// int ccw_addr, int enable_addr, int speed_addr,
// int encoder_a, int encoder_b)
Motor motor_br(0x20, 0x13, 14,    7, 2, 16, 17);
Motor motor_fr(0x20, 0x12, 112, 224, 3, 28, 27);
Motor motor_bl(0x24, 0x12, 112, 224, 10, 14, 15);
Motor motor_fl(0x24, 0x13, 14,    7, 29, 26, 25);
Dribbler dribbler(6);
Kicker kicker;
bool is_startup;

// Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
IntervalTimer PIDUpdateTimer;
Motion motion(motor_br, motor_fr, motor_bl, motor_fl);
XBEE xbee(5);

// Used to time how fast we send debug messages back through the xbee
unsigned long last_debug_timer = 0;

Command latestCommand;
// store timestamp of last setpoint update can expire after duration?
int last_command_ms;

void update_encoder_br_a() {
  motor_br.encoder.update_a();
}
void update_encoder_br_b() {
  motor_br.encoder.update_b();
}
void update_encoder_fr_a() {
  motor_fr.encoder.update_a();
}
void update_encoder_fr_b() {
  motor_fr.encoder.update_b();
}
void update_encoder_bl_a() {
  motor_bl.encoder.update_a();
}
void update_encoder_bl_b() {
  motor_bl.encoder.update_b();
}
void update_encoder_fl_a() {
  motor_fl.encoder.update_a();
}
void update_encoder_fl_b() {
  motor_fl.encoder.update_b();
}

void setup() {
  xbee.setup();
  motor_br.setup();
  motor_fr.setup();
  motor_bl.setup();
  motor_fl.setup();
  // Kicker will discharge by kicking for safety! Please watch out for your fingers!
  kicker.setup();

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

  if (IS_PID_INTERRUPT) {
    // use timer interrupts to make sure PID movement is being updated consistently
    // (interrupt rate must match with PID update loop rate for correct calculations)
    PIDUpdateTimer.begin(movePIDCallback, 1000000 / PID_UPDATE_HZ);
    // IMPORTANT - set interrupt to lowest priority, so encoder interrupts are not missed
    PIDUpdateTimer.priority(255);
  }

  // according to docs, baud rate is ignored
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  
  // Make sure our commands are zero'd after allocating!
  memset((void*) &latestCommand, 0, sizeof(Command));

  Serial.println("Starting up!");
  is_startup = true;
}

void movePIDCallback() {
  motion.update_PID();
}


void loop() {
  digitalWrite(LED, HIGH);
  // Serial.println("TEST");
  if (is_startup) {
    Serial.println("Starting up!");
    is_startup = false;
  }

  // Sample code to just spin the motors, in case we need to test out a new board.
  //  delay(100);
  //  digitalWrite(LED, LOW);
  //  delay(100);
  // motion.move_raw(0, 500, 0);
  //  delay(1000);
  //  motion.move_raw(0, 0, 0);
  //  delay(1000000000);
  // motion.XYW_to_setpoints(380, 320, 0);
  
  // stop everything if have not received command in a while
  if (millis() - last_command_ms > COMMAND_TIMEOUT_MILLIS) {
    latestCommand->is_dribbling = 0;
    latestCommand->is_charging = 0;
    latestCommand->is_kicking = 1;  // discharge for safety
    latestCommand->vx = 0;
    latestCommand->vy = 0;
    latestCommand->vw = 0;
  }

  if (millis() - last_debug_timer > DEBUG_REPORT_TIME && IS_DEBUG_REPORT) {
    last_debug_timer = millis();
    Serial.println("Debug Report!");
    // Serial.println(motion.PID_report());
    print_command_struct(&latestCommand);
    xbee.write_string(motion.PID_report());
  }
  // Read latest command from xbee into global buffer

  if (xbee.read_command(&latestCommand)) {
    // refresh the last command time
    last_command_ms = millis();
    // convert movement command into motor speed setpoints
    // (timer interrupt will trigger the actual PID updates)
    double x = latestCommand.vx;
    double y = latestCommand.vy;
    double w = latestCommand.vw;

    // Debug to see
    // motion.move_raw(0, 500, 0);

    motion.XYW_to_setpoints(x, y, w);

    // Serial.println("HELLO\n");

    if (latestCommand.is_dribbling) {
      dribbler.spin(255);
    } else {
      dribbler.spin(0);
    }

    if (latestCommand.is_charging) {
      // Serial.println("CHARGING!");
      kicker.force_charge();
    } else {
      kicker.force_charge_stop();
    }

    if (latestCommand.is_kicking) {
      kicker.force_kick();
      // Serial.println("KICKING");
    }
  }
  // Delay loop (avoiding using delay() function)
  int delay_time = 10;
  int start_time, end_time;
  start_time = end_time = millis();
  while (end_time - start_time < delay_time) {
    end_time = millis();
  }
}
