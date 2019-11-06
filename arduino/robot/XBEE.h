#ifndef XBEE_guard_h
#define XBEE_guard_h
enum CommandType { CMD_MOVE, CMD_DRIBBLE, CMD_KILL };

#include "Arduino.h"

#define XBEE_VERBOSE true
#define BUF_SZ 40

struct Command {
  bool is_dribbling;
  bool is_charging;
  bool is_kicking;
  double vx; // mm/second
  double vy; // mm/second
  double vw; // radians/second
};
typedef struct Command Command;

class XBEE {
public:
    XBEE(int);
    int id;
    bool read_line(Command*);
    void read_raw(char*);
    void setup();
    void write_string(String);
};

void print_command_struct(Command* cmd);
#endif
