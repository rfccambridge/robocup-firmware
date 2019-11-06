#include "Motion.h"
#include "Arduino.h"
// [OPTION] Limit total error to prevent integral windup in FastPid library:
// FastPid.h could have this:
// #define INTEG_MAX    C * PID_UPDATE_HZ
// #define INTEG_MIN    -C * PID_UPDATE_HZ
// Where C is the from 0-255 = max desired PWM contribution from integral
// Our I-tuning determines how fast the error builds up to the cap
#include "FastPID.h"

Motion::Motion(Motor& br_motor, Motor& fr_motor, Motor& bl_motor, Motor& fl_motor) 
               : br(br_motor), fr(fr_motor), bl(bl_motor), fl(fl_motor) {
}

void Motion::setup(float k_p, float k_i, float k_d) {
    // Pass in the proportional, integral, and derivative constants
    // Also pass in hz (times/s) at which move function will be called 
    // '8' argument refers to output bits, which is from our pwm going to 255
    pid_br = new FastPID(k_p, k_i, k_d, PID_UPDATE_HZ, 8, true);
    pid_fr = new FastPID(k_p, k_i, k_d, PID_UPDATE_HZ, 8, true);
    pid_bl = new FastPID(k_p, k_i, k_d, PID_UPDATE_HZ, 8, true);
    pid_fl = new FastPID(k_p, k_i, k_d, PID_UPDATE_HZ, 8, true);
    // reset motor encoder counts
    br.reset_position();
    fr.reset_position();
    bl.reset_position();
    fl.reset_position();
}

void Motion::stop() {
    bl.stop();
    fl.stop();
    br.stop();
    fr.stop();
}

void Motion::move_raw(double x, double y, double w) {
  int power_bl = (int) (-x * sin(THETA) + y * cos(THETA) + w);
  int power_fl = (int) (x * sin(THETA) + y * cos(THETA) + w);
  int power_fr = (int) (x * sin(THETA) - y * cos(THETA) + w);
  int power_br = (int) (-x * sin(THETA) - y * cos(THETA) + w);
  bl.turn(power_bl);
  fl.turn(power_fl);
  fr.turn(power_fr);
  br.turn(power_br);
}

// converts radians of the robot to rps of wheel
double Motion::radians_to_rps(double rad) {
  return rad * (ROBOT_DIAMETER / 2) / (MM_PER_ROTATION);
}

// Converts xyw (mm/s for x and y, rad/s for w) => setpoints (ticks/pid interval)
// This keeps update_PID function as quick as possible
// (which is good b.c it runs repeatedly for the same setpoints)
void Motion::XYW_to_setpoints(double x, double y, double w) {
  // TODO: COULD LIMIT SET POINT CHANGE RATE - SEE ZHEJIANG FIRMWARE GITHUB
  // First, calculate wheel speeds to move in correct x/y direction vector
  double rps_bl = -x/MM_PER_ROTATION / sin(THETA) + y/MM_PER_ROTATION / cos(THETA);
  double rps_fl = x/MM_PER_ROTATION / sin(THETA) + y/MM_PER_ROTATION / cos(THETA);
  double rps_fr = x/MM_PER_ROTATION / sin(THETA) - y/MM_PER_ROTATION / cos(THETA);
  double rps_br = -x/MM_PER_ROTATION / sin(THETA) - y/MM_PER_ROTATION / cos(THETA);
  // division compensates for inefficies to normalize at given x,y for whole robot
  
  // add the robot rotation speed to every wheel
  rps_bl += radians_to_rps(w);
  rps_fl += radians_to_rps(w);
  rps_fr += radians_to_rps(w);
  rps_br += radians_to_rps(w);
  
  // We are forced to round speeds to multiple of 1 tick per update interval
  // i.e. 400hz / 465 ticks per rev = rounds speeds to multiple of .86 rotations/second
  // i.e. 200hz / 465 ticks per rev = rounds speeds to multiple of .43 rotations/second
  // Too big means software cannot control the robots properly 8( NEED FINER ENCODERS!
  // TODO: should we round small speeds up to 1 rather than down to 0?
  
  setpoint_bl = (int) (PID_SCALE*rps_bl * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_fl = (int) (PID_SCALE*rps_fl * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_fr = (int) (PID_SCALE*rps_fr * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_br = (int) (PID_SCALE*rps_br * TICKS_PER_REV / PID_UPDATE_HZ);

  // Serial.print("BL Setpoint:");
  // Serial.println(setpoint_bl);
 
  // refresh the last command time
  last_command_ms = millis();
}

void Motion::update_PID() {
    int pid_bl_in = PID_SCALE*bl.position_ticks();
    int pid_fl_in = PID_SCALE*fl.position_ticks();
    int pid_br_in = PID_SCALE*br.position_ticks();
    int pid_fr_in = PID_SCALE*fr.position_ticks();

    // reset encoder counts
    bl.reset_position();
    fl.reset_position();
    fr.reset_position();
    br.reset_position();

    // Serial.print("BL Setpoint:");
    // Serial.println(setpoint_bl);
    // Serial.print("BL Ticks/Interval:");
    //Serial.println(pid_bl_in);
    //Serial.println(pid_br_in);
    //Serial.println(pid_fl_in);
    // Serial.println(pid_fr_in);
    
    pid_bl_out = pid_bl->step(setpoint_bl, pid_bl_in);
    pid_fl_out = pid_fl->step(setpoint_fl, pid_fl_in);
    pid_br_out = pid_br->step(setpoint_br, pid_br_in);
    pid_fr_out = pid_fr->step(setpoint_fr, pid_fr_in);

    if (millis() - last_command_ms > TIMEOUT_MILLIS) {
      stop();
      return;
    }
    
    bl.turn(pid_bl_out);
    br.turn(pid_br_out);
    fl.turn(pid_fl_out);
    fr.turn(pid_fr_out);
    Serial.print(" ");
}


/*
 * Reports back the PID debug statistics in the following format
 * String as a python tuple (fl, fr, bl, br), where each motor is:
 * (unsigned int (milliseconds since start), 
 *  motorid (str),
 *  setpoint (int),
 *  input (ticks/interval),
 *  output (pwm))
 */
String Motion::PID_report() {
  // TODO: right now the inputs are inaccurate because not being called at the exact end of the PID update intervals!
    char report[80];
    snprintf(
      report, 
      80, 
      "((%d, %d, %d), (%d, %d, %d), (%d, %d, %d), (%d, %d, %d))",
      setpoint_fl,
      PID_SCALE * fl.position_ticks(),
      pid_fl_out,
      setpoint_fr,
      PID_SCALE * fr.position_ticks(),
      pid_fr_out,
      setpoint_bl,
      PID_SCALE * bl.position_ticks(),
      pid_bl_out,
      setpoint_br,
      PID_SCALE * br.position_ticks(),
      pid_br_out
    );
    return String(report);
}
