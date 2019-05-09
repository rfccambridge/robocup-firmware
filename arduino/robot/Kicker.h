#ifndef Kicker_h
#define Kicker_h

#include "Arduino.h"


class Kicker {
public:
    Kicker();
    void kick();
    void discharge();
private:
    double charge;
};

#endif
