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
    void stop();
private:
    Motor &br;
    Motor &fr;
    Motor &bl;
    Motor &fl;
};

#endif
