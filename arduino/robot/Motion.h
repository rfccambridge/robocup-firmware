#ifndef Motion_h
#define Motion_h

#include "Arduino.h"

#define ROTATION_SCALE 1
#define THETA 75.0 / 2 * PI / 180
#define BR1 8
#define BR2 7
#define BR_SPEED 10
#define FR1 4
#define FR2 2
#define FR_SPEED 9
#define FL1 51
#define FL2 49
#define FL_SPEED 45
#define BL1 43
#define BL2 42
#define BL_SPEED 46 
#define SPEED_CAP 100

class Motion {
public:
    Motion();
    void move(int*);
    void setup();
private:
    void stop_all();
    void make_move(int*);
    void transformation(int*, int*);
};

#endif
