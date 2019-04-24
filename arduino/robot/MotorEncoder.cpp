#include "Arduino.h"
#include "MotorEncoder.h"
#include "Encoder.h"

MotorEncoder::MotorEncoder(int encoder_a_pin, int encoder_b_pin) {
    encoder_a = encoder_a_pin;
    encoder_b = encoder_b_pin;
    pos = 0;
    pinMode(encoder_a, INPUT);
    pinMode(encoder_b, INPUT);
}

int MotorEncoder::read() {
    return pos;
}

void MotorEncoder::write(int new_pos) {
    pos = new_pos;
}


void MotorEncoder::update_a() {
  pos += (digitalRead(encoder_a) == digitalRead(encoder_b)) ? -1 : 1;
}

void MotorEncoder::update_b() {
   pos += (digitalRead(encoder_a) == digitalRead(encoder_b)) ? 1 : -1;
}