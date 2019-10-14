#include "Arduino.h"
#include "Motor.h"

#define MOTOR_LOW_LIMIT 1

Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, int ccw_addr, int spd_addr, 
             int encoder_a, int encoder_b)
    : encoder(encoder_a, encoder_b) {
    // pins are 1 indexed for CW, CCW, enable
    mcp = mcp_addr;
    motor = motor_addr;
    cw = cw_addr;
    ccw = ccw_addr;
    spd = spd_addr;
}

void Motor::setup() {
    Wire.begin();
    Wire.beginTransmission(mcp);
    Wire.write(0x00); // IODIRA register
    Wire.write(0x00); // set all of port A to outputs
    Wire.endTransmission();
    Wire.beginTransmission(mcp);
    Wire.write(0x01); // IODIRB register
    Wire.write(0x00); // set all of port B to outputs
    Wire.endTransmission();
    pinMode(spd, OUTPUT);
    encoder.write(0);
}

void Motor::turn(int turn_spd) {
    if (abs(turn_spd) < MOTOR_LOW_LIMIT) {
      turn_spd = 0;
    }
    unsigned int command = 0;
    if (turn_spd != 0) {
      command |= (turn_spd > 0) ? cw : ccw;
    }
    analogWrite(spd, abs(turn_spd));

    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(command);
    Wire.endTransmission();
}

int Motor::position_ticks() {
    return encoder.read();
}

double Motor::position_revs() {
    return (double)encoder.read() / TICKS_PER_REV;
}

void Motor::reset_position() {
    encoder.write(0);
}
/*
void Motor::fastbreak() {

}
*/

void Motor::stop() {
    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(0);
    Wire.endTransmission();
}
