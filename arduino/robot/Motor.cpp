#include "Arduino.h"
#include "Motor.h"

Motor::Motor(int mcp_addr, int motor_addr, int cw_addr, int ccw_addr, int enable_addr, int speed_addr, int encoder_a, int encoder_b)
    : encoder(encoder_a, encoder_b), pid(&pid_input, &pid_output, &pid_set, 1, 0, 0, DIRECT) {
    // pins are 1 indexed for CW, CCW, enable
    mcp = mcp_addr;
    motor = motor_addr;
    cw = cw_addr;
    ccw = ccw_addr;
    enable = enable_addr;
    speed = speed_addr;
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
    pinMode(speed, OUTPUT);
    encoder.write(0);
    pid.SetMode(AUTOMATIC);
    Serial.begin(9600);
}

void Motor::turn(int turn_speed) {
    pid_set = turn_speed;
    pid_input = encoder.read();
    encoder.write(0);
    pid.Compute();
    int command = (1 << (enable - 1)) | (1 << (((pid_output > 0) ? cw : ccw) - 1)); 

    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(command);
    Wire.endTransmission();
    analogWrite(speed, abs(pid_output));
}

void Motor::stop() {
    Wire.beginTransmission(mcp);
    Wire.write(motor); 
    Wire.write(0);
    Wire.endTransmission();
}
