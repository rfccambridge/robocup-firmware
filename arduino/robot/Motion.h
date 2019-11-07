#ifndef Motion_h
#define Motion_h
#include "Arduino.h"
#include "Motor.h"
#include "FastPID.h"

#define THETA ((75.0 / 2) * (PI / 180))
#define TICKS_HISTORY_SIZE 2

#define PID_UPDATE_HZ 500
// how long commands are followed for (time to live)
#define TIMEOUT_MILLIS 500
#define MM_PER_ROTATION (45*PI)
#define ROBOT_DIAMETER 165
// multiplies inputs and setpoints by a constant value to allow for increased setpoint granularity
#define PID_SCALE 100

// unused?
#define CLIP(x, min_x, max_x) max(min(x, max_x), min_x)

class Motion {
public:
    Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
    void move_raw(double, double, double);
    void XYW_to_setpoints(double, double, double);
    void debug_move_command(double, double, double);
    void update_PID();
    void setup(float k_p, float k_i, float k_d);
    void stop();
    String PID_report();
private:
    double radians_to_rps(double);
    Motor &br;
    Motor &fr;
    Motor &bl;
    Motor &fl;
    int setpoint_bl;
    int setpoint_fl;
    int setpoint_fr;
    int setpoint_br;
    int pid_bl_out;
    int pid_fl_out;
    int pid_fr_out;
    int pid_br_out;
    FastPID *pid_br, *pid_fr, *pid_bl, *pid_fl;
    // store timestamp of last setpoint update can expire after duration?
    int last_command_ms;
};

#endif
