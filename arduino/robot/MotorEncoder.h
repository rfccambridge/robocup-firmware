#ifndef MotorEncoder_h
#define MotorEncoder_h

#include "Arduino.h"
#include "Encoder.h"


class MotorEncoder {
public:
    MotorEncoder(int, int);
    int read();
    void write(int);
    int encoder_a;
    int encoder_b;
    void update_a();
    void update_b();
private:
    int pos;
};

#endif
