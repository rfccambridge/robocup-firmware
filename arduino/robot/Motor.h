#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Wire.h"
#include "MotorEncoder.h"
#include "PID_v1.h"
// Sometimes motors are reversed?
// For now polarity = 1 is shaft counterclockwise for positive pwm
#define MOTOR_POLARITY -1

#define SPEED_CAP 255
#define TICKS_PER_REV 980

class Motor {
public:
    Motor(int, int, int, int, int, int, int);
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
    int spd;
};

#endif
