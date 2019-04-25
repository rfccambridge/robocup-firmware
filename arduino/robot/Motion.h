#ifndef Motion_h
#define Motion_h

#include "Arduino.h"
#include "Motor.h"

#define ROTATION_SCALE 1
#define THETA 75.0 / 2 * PI / 180

class Motion {
public:
    Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
    void move(int, int, int);
    void setup(double k_p, double k_i, double k_d);
    void stop();
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
    unsigned long time_ms;
};

#endif
