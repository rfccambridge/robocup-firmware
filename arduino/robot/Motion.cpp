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

// Converts xyw (rotations/s) => setpoints (ticks/pid interval)
// This keeps update_PID function as quick as possible
// (which is good b.c it runs repeatedly for the same setpoints)
void Motion::XYW_to_setpoints(double x, double y, double w) {
  double rps_bl = -x * sin(THETA) + y * cos(THETA) + w;
  double rps_fl = x * sin(THETA) + y * cos(THETA) + w;
  double rps_fr = x * sin(THETA) - y * cos(THETA) + w;
  double rps_br = -x * sin(THETA) - y * cos(THETA) + w;
  // We are forced to round speeds to multiple of 1 tick per update interval
  // i.e. 400hz / 465 ticks per rev = rounds speeds to multiple of .86 rotations/second
  // i.e. 200hz / 465 ticks per rev = rounds speeds to multiple of .43 rotations/second
  // Too big means software cannot control the robots properly 8( NEED FINER ENCODERS!
  // TODO: should we round small speeds up to 1 rather than down to 0?
  setpoint_bl = (int) (rps_bl * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_fl = (int) (rps_fl * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_fr = (int) (rps_fr * TICKS_PER_REV / PID_UPDATE_HZ);
  setpoint_br = (int) (rps_br * TICKS_PER_REV / PID_UPDATE_HZ);
  // refresh the last command time
  last_command_ms = millis();
}

void Motion::update_PID() {
    if (millis() - last_command_ms > TIMEOUT_MILLIS) {
      stop();
      return;
    }
    int pid_bl_in = bl.position_ticks();
    int pid_fl_in = fl.position_ticks();
    int pid_br_in = br.position_ticks();
    int pid_fr_in = fr.position_ticks();

    int pid_bl_out = pid_bl->step(setpoint_bl, pid_bl_in);
    int pid_fl_out = pid_fl->step(setpoint_fl, pid_fl_in);
    int pid_br_out = pid_br->step(setpoint_br, pid_br_in);
    int pid_fr_out = pid_fr->step(setpoint_fr, pid_fr_in);

    bl.turn(pid_bl_out);
    br.turn(pid_br_out);
    fl.turn(pid_fl_out);
    fr.turn(pid_fr_out);

    // reset encoder counts
    bl.reset_position();
    fl.reset_position();
    fr.reset_position();
    br.reset_position();
}
