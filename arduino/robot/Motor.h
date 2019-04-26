#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Wire.h"
#include "MotorEncoder.h"
#include "PID_v1.h"

#define SPEED_CAP 255
#define TICKS_PER_REV 465

class Motor {
public:
    Motor(int, int, int, int, int, int, int, int);
    void turn(int);
    void setup();
    void stop();
    int position_ticks();
    double position_revs();
    void reset_position();
    MotorEncoder encoder;
private:
    int mcp;
    int motor;
    int cw;
    int ccw;
    int enable;
    int spd;
};

#endif
