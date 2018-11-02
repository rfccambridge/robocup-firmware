#ifndef XBEE_guard_h
#define XBEE_guard_h

#include "Arduino.h"

#define XBEE_VERBOSE true
#define BUF_SZ 40

class XBEE {
public:
    XBEE(int);
    int id;
    void read_line(int*);
    void setup();
};

#endif
