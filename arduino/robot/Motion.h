#ifndef Motion_h
#define Motion_h
#include "Arduino.h"
#include "Motor.h"
#include "FastPID.h"
#include "CircularBuffer.h"

#define THETA 75.0 / 2 * PI / 180
#define TICKS_HISTORY_SIZE 2

#define TICKS_PER_REV 465

// unused?
#define CLIP(x, min_x, max_x) max(min(x, max_x), min_x)

class Motion {
public:
    Motion(Motor& br, Motor& fr, Motor& bl, Motor& fl);
    void move_raw(double, double, double);
    void XYW_to_setpoints(double, double, double, int*);
    void move_PID(int, int, int, int);
    void setup(float k_p, float k_i, float k_d, float hz);
    void stop();
    void debug();
private:
    Motor &br;
    Motor &fr;
    Motor &bl;
    Motor &fl;
    FastPID *pid_br, *pid_fr, *pid_bl, *pid_fl;
    int update_hz;
    // store ticks readings to allow using larger window for estimating slower speeds
    CircularBuffer<int, TICKS_HISTORY_SIZE> br_ticks_history;
    CircularBuffer<int, TICKS_HISTORY_SIZE> fr_ticks_history;
    CircularBuffer<int, TICKS_HISTORY_SIZE> bl_ticks_history;
    CircularBuffer<int, TICKS_HISTORY_SIZE> fl_ticks_history;
    int sum_last_intervals(CircularBuffer<int, TICKS_HISTORY_SIZE>*, int);
};

#endif
