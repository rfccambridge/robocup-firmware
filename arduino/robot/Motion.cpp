#include "Motion.h"
#include "Arduino.h"
#include "PID_v1.h"

// #define MOTION_DEBUGGING

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
    pid_br = new PID(&pid_br_in, &pid_br_out, &setpoint_br, 0, 0, 0, DIRECT);
    pid_fr = new PID(&pid_fr_in, &pid_fr_out, &setpoint_fr, 0, 0, 0, DIRECT);
    pid_bl = new PID(&pid_bl_in, &pid_bl_out, &setpoint_bl, 0, 0, 0, DIRECT);
    pid_fl = new PID(&pid_fl_in, &pid_fl_out, &setpoint_fl, 0, 0, 0, DIRECT);
    pid_br->SetTunings(k_p, k_i, k_d);
    pid_fr->SetTunings(k_p, k_i, k_d);
    pid_bl->SetTunings(k_p, k_i, k_d);
    pid_fl->SetTunings(k_p, k_i, k_d);
    pid_br->SetMode(AUTOMATIC);
    pid_fr->SetMode(AUTOMATIC);
    pid_bl->SetMode(AUTOMATIC);
    pid_fl->SetMode(AUTOMATIC);
    br.reset_position();
    fr.reset_position();
    bl.reset_position();
    fl.reset_position();
    time_ms = millis();
}

void Motion::stop() {
    bl.stop();
    fl.stop();
    br.stop();
    fr.stop(); 
}

void Motion::move(int x, int y, int w) {
    double delta_time = millis() - time_ms;
    if (delta_time == 0) {
        pid_br_in = 0;
        pid_fl_in = 0;
        pid_br_in = 0;
        pid_fr_in = 0;
    } else {
        // units we will be using is revolutions per second
        pid_br_in = ((double) bl.position()) / delta_time * 1000.0;
        pid_fl_in = ((double) fl.position()) / delta_time * 1000.0;
        pid_br_in = ((double) br.position()) / delta_time * 1000.0;
        pid_fr_in = ((double) fr.position()) / delta_time * 1000.0;
    }
    // """Pass in movements in revolutions per second now"""
    setpoint_bl = (-x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_fl = (x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_fr = (x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    setpoint_br = (-x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    pid_bl->Compute();
    pid_fl->Compute();
    pid_fr->Compute();
    pid_br->Compute();

    bl.turn(pid_bl_out);
    br.turn(pid_br_out);
    fl.turn(pid_fl_out);
    fr.turn(pid_fr_out);

    #ifdef MOTION_DEBUGGING
        Serial.print(pid_br_in);
        Serial.print("       ");
        Serial.print(pid_br_out);
        Serial.print("       ");
        Serial.println(setpoint_br);
        
        Serial.print(pid_bl_in);
        Serial.print("       ");
        Serial.print(pid_bl_out);
        Serial.print("       ");
        Serial.println(setpoint_bl);
        
        Serial.print(pid_fr_in);
        Serial.print("       ");
        Serial.print(pid_fr_out);
        Serial.print("       ");
        Serial.println(setpoint_fr);
        
        Serial.print(pid_fl_in);
        Serial.print("       ");
        Serial.print(pid_fl_out);
        Serial.print("       ");
        Serial.println(setpoint_fl);
        Serial.println("=====================");
    #endif // DEBUG

    // reset encoder counts and get time again
    bl.reset_position();
    fl.reset_position();
    fr.reset_position();
    br.reset_position();
    time_ms = millis();
}
