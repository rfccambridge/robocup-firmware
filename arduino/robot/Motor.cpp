#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, int ccw_addr, int enable_addr,
             int spd_addr, int encoder_a, int encoder_b)
    : encoder(encoder_a, encoder_b), pid(&pid_input, &pid_output, &pid_set, 1, 0, 0, DIRECT) {
    // pins are 1 indexed for CW, CCW, enable
    mcp = mcp_addr;
    motor = motor_addr;
    cw = cw_addr;
    ccw = ccw_addr;
    enable = enable_addr;
    spd = spd_addr;
    pid.SetOutputLimits(-SPEED_CAP, SPEED_CAP);
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
    pid.SetMode(AUTOMATIC);
}

void Motor::turn(int turn_spd) {
    unsigned int command = 0;
    if (turn_spd != 0) {
      command |= (turn_spd > 0) ? cw : ccw;
    }
        analogWrite(spd, abs(turn_spd));

    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(command);
    Wire.endTransmission();
    // analogWrite(spd, abs(pid_output));
}

int Motor::position() {
    return encoder.read();
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
