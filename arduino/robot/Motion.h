#ifndef Motion_h
#define Motion_h
#include "Arduino.h"
#include "Motor.h"
#include "FastPID.h"

#define THETA 75.0 / 2 * PI / 180
#define TICKS_HISTORY_SIZE 2

#define TICKS_PER_REV 465

// unused?
#define CLIP(x, min_x, max_x) max(min(x, max_x), min_x)

class Motion {
public:
    Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
    void move_raw(double, double, double);
    void XYW_to_setpoints(double, double, double);
    void update_PID();
    void setup(float k_p, float k_i, float k_d, float hz);
    void stop();
    void debug();
private:
    Motor &br;
    Motor &fr;
    Motor &bl;
    Motor &fl;
    int setpoint_bl;
    int setpoint_fl;
    int setpoint_fr;
    int setpoint_br;
    // TODO: store timestamp, so we can expire after .2 seconds or so
    FastPID *pid_br, *pid_fr, *pid_bl, *pid_fl;
    int update_hz;
};

#endif
