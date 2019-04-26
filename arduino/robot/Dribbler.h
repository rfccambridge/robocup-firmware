#ifndef Dribbler_h
#define Dribbler_h

#include "Arduino.h"

class Dribbler {
public:
    Dribbler(int);
    void spin(int);
    void stop();
private:
    int pin;
};

#endif
