#include "Motion.h"
#include "Arduino.h"
#include "PID_v1.h"

// #define MOTION_DEBUGGING
#define KI_LIMIT 1.0
#define PWM_LIMIT 255

Motion::Motion(Motor& br_motor, Motor& fr_motor, Motor& bl_motor, Motor& fl_motor) 
               : br(br_motor), fr(fr_motor), bl(bl_motor), fl(fl_motor) {
}

void Motion::setup(double k_p, double k_i, double k_d) {
    // Pass in the proportional, integral, and derivative constants
    // default 0.1, 2, 0.1
    setpoint_br = 0;
    setpoint_fr = 0;
    setpoint_bl = 0;
    setpoint_fl = 0;
    pid_br_in = 0;
    pid_fr_in = 0;
    pid_bl_in = 0;
    pid_fl_in = 0;
    pid_br_out = 0;
    pid_fr_out = 0;
    pid_bl_out = 0;
    pid_fl_out = 0;
    pid_br_ek = 0;
    pid_fr_ek = 0;
    pid_fl_ek = 0;
    pid_bl_ek = 0;
    pid_br_ks = 0;
    pid_fr_ks = 0;
    pid_fl_ks = 0;
    pid_bl_ks = 0;
    // pid_br = new PID(&pid_br_in, &pid_br_out, &setpoint_br, 0, 0, 0, DIRECT);
    // pid_fr = new PID(&pid_fr_in, &pid_fr_out, &setpoint_fr, 0, 0, 0, DIRECT);
    // pid_bl = new PID(&pid_bl_in, &pid_bl_out, &setpoint_bl, 0, 0, 0, DIRECT);
    // pid_fl = new PID(&pid_fl_in, &pid_fl_out, &setpoint_fl, 0, 0, 0, DIRECT);
    // pid_br->SetTunings(k_p, k_i, k_d);
    // pid_fr->SetTunings(k_p, k_i, k_d);
    // pid_bl->SetTunings(k_p, k_i, k_d);
    // pid_fl->SetTunings(k_p, k_i, k_d);
    // pid_br->SetMode(AUTOMATIC);
    // pid_fr->SetMode(AUTOMATIC);
    // pid_bl->SetMode(AUTOMATIC);
    // pid_fl->SetMode(AUTOMATIC);
    // pid_br->SetOutputLimits(-0, 255);
    // pid_fr->SetOutputLimits(-0, 255);
    // pid_bl->SetOutputLimits(-0, 255);
    // pid_fl->SetOutputLimits(-0, 255);
    br.reset_position();
    fr.reset_position();
    bl.reset_position();
    fl.reset_position();
    pid_kp = k_p;
    pid_ki = k_i;
    time_micros = micros();
}

void Motion::stop() {
    bl.stop();
    fl.stop();
    br.stop();
    fr.stop(); 
}

void Motion::move_raw(double x, double y, double w) {
  int power_bl = (int) (-x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
  int power_fl = (int) (x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
  int power_fr = (int) (x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
  int power_br = (int) (-x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
  bl.turn(power_bl);
  fl.turn(power_fl);
  fr.turn(power_fr);
  br.turn(power_br);
}

void Motion::move(double x, double y, double w) {
    double delta_seconds;
    // Need to make this a reasonable time window for encoder readings
    delta_seconds = (micros() - time_micros) / 1000000.0;
    // input encoder speed measurements (revolutions per second)
    pid_bl_in = bl.position_revs() / delta_seconds; 
    pid_fl_in = fl.position_revs() / delta_seconds; 
    pid_br_in = br.position_revs() / delta_seconds; 
    pid_fr_in = fr.position_revs() / delta_seconds; 
   
    // set desired rotation speeds (revolutions per second now)
    setpoint_bl = (-x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_fl = (x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_fr = (x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_br = (-x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    // pid_bl->Compute();
    // pid_fl->Compute();
    // pid_fr->Compute();
    // pid_br->Compute();

    pid_bl_ek = setpoint_bl - pid_bl_in;
    pid_fl_ek = setpoint_fl - pid_fl_in;
    pid_br_ek = setpoint_br - pid_br_in;
    pid_fr_ek = setpoint_fr - pid_fr_in;

    // pid_bl_ek = setpoint_bl;
    // pid_fl_ek = setpoint_fl;
    // pid_br_ek = setpoint_br;
    // pid_fr_ek = setpoint_fr;

    pid_bl_ks += pid_bl_ek;
    pid_fl_ks += pid_fl_ek;
    pid_br_ks += pid_br_ek;
    pid_fr_ks += pid_fr_ek;

    // clip integral factor at a limit
    // TODO: reset accumulations (ks) once a new command is given!!!
    pid_bl_ks = CLIP(pid_bl_ks, -KI_LIMIT, KI_LIMIT);
    pid_fl_ks = CLIP(pid_fl_ks, -KI_LIMIT, KI_LIMIT);
    pid_br_ks = CLIP(pid_br_ks, -KI_LIMIT, KI_LIMIT);
    pid_fr_ks = CLIP(pid_fr_ks, -KI_LIMIT, KI_LIMIT);


    pid_bl_out = pid_bl_ek * pid_kp + pid_bl_ks * pid_ki;
    pid_fl_out = pid_fl_ek * pid_kp + pid_fl_ks * pid_ki;
    pid_br_out = pid_br_ek * pid_kp + pid_br_ks * pid_ki;
    pid_fr_out = pid_fr_ek * pid_kp + pid_fr_ks * pid_ki;

    pid_bl_ks = CLIP(pid_bl_out, -PWM_LIMIT, PWM_LIMIT);
    pid_fl_ks = CLIP(pid_fl_out, -PWM_LIMIT, PWM_LIMIT);
    pid_br_ks = CLIP(pid_br_out, -PWM_LIMIT, PWM_LIMIT);
    pid_fr_ks = CLIP(pid_fr_out, -PWM_LIMIT, PWM_LIMIT);

    // pid_bl_out = max(pid_bl_out, 0);

    bl.turn(pid_bl_out);
    br.turn(pid_br_out);
    fl.turn(pid_fl_out);
    fr.turn(pid_fr_out);

    #ifdef MOTION_DEBUGGING
        debug();
    #endif // DEBUG

    // reset encoder counts and get time again
    bl.reset_position();
    fl.reset_position();
    fr.reset_position();
    br.reset_position();
    time_micros = micros();
}

void Motion::debug() {
  /*Serial.println(br.position_revs());
        Serial.println(bl.position_revs());
        Serial.println(fr.position_revs());
        Serial.println(fl.position_revs());*/
        Serial.print("BR");
        Serial.print("       ");
        Serial.print(pid_br_in);
        Serial.print("       ");
        Serial.print(pid_br_out);
        Serial.print("       ");
        Serial.println(setpoint_br);
        
        Serial.print("BL");
        Serial.print("       ");
        Serial.print(pid_bl_in);
        Serial.print("       ");
        Serial.print(pid_bl_out);
        Serial.print("       ");
        Serial.println(setpoint_bl);
        
        Serial.print("FR");
        Serial.print("       ");
        Serial.print(pid_fr_in);
        Serial.print("       ");
        Serial.print(pid_fr_out);
        Serial.print("       ");
        Serial.println(setpoint_fr);
        
        Serial.print("FL");
        Serial.print("       ");
        Serial.print(pid_fl_in);
        Serial.print("       ");
        Serial.print(pid_fl_out);
        Serial.print("       ");
        Serial.println(setpoint_fl);
        Serial.println("=====================");
}
