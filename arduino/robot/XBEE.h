#ifndef XBEE_guard_h
#define XBEE_guard_h
enum CommandType { CMD_MOVE, CMD_DRIBBLE };

#include "Arduino.h"

#define XBEE_VERBOSE true
#define BUF_SZ 40

class XBEE {
public:
    XBEE(int);
    int id;
    void read_line(int*);
    void read_raw(char*);
    void setup();
};

#endif
