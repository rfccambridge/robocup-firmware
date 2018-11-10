#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Wire.h"
#include "Encoder.h"
#include "PID_v1.h"

#define SPEED_CAP 255

class Motor {
public:
    Motor(int, int, int, int, int, int, int, int);
    void turn(int);
    void setup();
    void stop();
private:
    int mcp;
    int motor;
    int cw;
    int ccw;
    int enable;
    int speed;
    Encoder encoder;
    double pid_input;
    double pid_output;
    double pid_set;
    PID pid;
};

#endif
