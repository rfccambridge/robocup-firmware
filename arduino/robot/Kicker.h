#ifndef Kicker_h
#define Kicker_h

#include "Arduino.h"


class Kicker {
public:
    Kicker();
    // wati and kick until we get to a certain charge - don't kick unless 
    // we are at the charge level specified +- 5%
    void kick(double charge_level);
    // force kick - try kicking no matter how charged the capacitor is.
    void force_kick();
    // Tries to discharge the capacitors without kicking for safety reasons
    void discharge();
    // Keeps the capacitor charged to a certain percentage - makes it faster
    // to kick at the right speed.
    void set_charge(double charge_level);
private:
    // Percentage of capacitor charge - should be proportional to kick power.
    // double between 0 and 1.
    double charge;
};

#endif
