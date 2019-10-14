#ifndef Motion_h
#define Motion_h

#include "Arduino.h"
#include "Motor.h"

#define ROTATION_SCALE 1
#define THETA 75.0 / 2 * PI / 180

#define TICKS_PER_REV 465

#define CLIP(x, min_x, max_x) max(min(x, max_x), min_x)

class Motion {
public:
    Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
    void move_raw(double, double, double);
    void move(double, double, double);
    void setup(double k_p, double k_i, double k_d);
    void stop();
    void debug();
private:
    Motor &br;
    Motor &fr;
    Motor &bl;
    Motor &fl;
    PID *pid_br, *pid_fr, *pid_bl, *pid_fl;
    double setpoint_br;
    double setpoint_fr;
    double setpoint_bl;
    double setpoint_fl;
    double pid_br_in;
    double pid_fr_in;
    double pid_bl_in;
    double pid_fl_in;
    double pid_br_out;
    double pid_fr_out;
    double pid_bl_out;
    double pid_fl_out;
    double pid_kp;
    double pid_ki;
    double pid_br_ek;
    double pid_fr_ek;
    double pid_fl_ek;
    double pid_bl_ek;
    double pid_br_ks;
    double pid_fr_ks;
    double pid_fl_ks;
    double pid_bl_ks;
    unsigned long time_micros;
};

#endif
