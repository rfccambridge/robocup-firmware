#include "Dribbler.h"

Dribbler::Dribbler(int pin_addr) {
    // single digital pin for on/off
    pin = pin_addr;
    pinMode(pin, OUTPUT);
}

void Dribbler::spin(int speed) {
    analogWrite(pin, speed);
}

void Dribbler::stop() {
    analogWrite(pin, 0);
}
