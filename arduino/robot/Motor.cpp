#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, int ccw_addr, int enable_addr, int speed_addr) {
    // pins are 1 indexed for CW, CCW, enable
    mcp = mcp_addr;
    motor = motor_addr;
    cw = cw_addr;
    ccw = ccw_addr;
    enable = enable_addr;
    speed = speed_addr;
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
    pinMode(speed, OUTPUT);
}

void Motor::turn(int turn_speed) {
    int command = (1 << (enable - 1)) | (1 << (((turn_speed > 0) ? cw : ccw) - 1)); 
    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(command);
    Wire.endTransmission();
    analogWrite(speed, min(abs(turn_speed), SPEED_CAP));
}

void Motor::stop() {
    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(0);
    Wire.endTransmission();
}
