#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Wire.h"

#define SPEED_CAP 255

class Motor {
public:
    Motor(int, int, int, int, int, int);
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
};

#endif
