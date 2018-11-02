#include "Motion.h"
#include "Arduino.h"

Motion::Motion(){}

void Motion::setup(){
    pinMode(BR1, OUTPUT);
    pinMode(BR2, OUTPUT);
    pinMode(FR1, OUTPUT);
    pinMode(FR2, OUTPUT);
    pinMode(FL1, OUTPUT);
    pinMode(FL2, OUTPUT);
    pinMode(BL1, OUTPUT);
    pinMode(BL2, OUTPUT);

    pinMode(FR_SPEED, OUTPUT);
    pinMode(BR_SPEED, OUTPUT);
    pinMode(FL_SPEED, OUTPUT);
    pinMode(BL_SPEED, OUTPUT);
    analogWrite(FR_SPEED, 0);
    analogWrite(BR_SPEED, 0); 
    analogWrite(FL_SPEED, 0);    
    analogWrite(BL_SPEED, 0);
}

void Motion::move(int* v){
    if (v[0] == 0 && v[1] == 0 && v[2] == 0){
        stop_all();
    } else {
        int transformed_v[4];
        transformation(v, transformed_v);
        make_move(transformed_v);
    }
}

void Motion::transformation(int* v, int* result){
  int x = v[0];
  int y = v[1];
  result[0] = x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * v[2];
  result[1] = -x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * v[2];
  result[2] = -x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * v[2];
  result[3] = x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * v[2];
}

void Motion::make_move(int* v) {
    analogWrite(BL_SPEED, min(abs(v[0]), SPEED_CAP));
    digitalWrite(BL1, (v[0] > 0) ? HIGH : LOW);
    digitalWrite(BL2, (v[0] < 0) ? HIGH : LOW);
    analogWrite(FL_SPEED, min(abs(v[1]), SPEED_CAP));
    digitalWrite(FL1, (v[1] > 0) ? HIGH : LOW);
    digitalWrite(FL2, (v[1] < 0) ? HIGH : LOW);   
    analogWrite(FR_SPEED, min(abs(v[2]), SPEED_CAP));
    digitalWrite(FR1, (v[2] > 0) ? HIGH : LOW);
    digitalWrite(FR2, (v[2] < 0) ? HIGH : LOW);
    analogWrite(BR_SPEED, min(abs(v[3]), SPEED_CAP));
    digitalWrite(BR1, (v[3] > 0) ? HIGH : LOW);
    digitalWrite(BR2, (v[3] < 0) ? HIGH : LOW);
}

void Motion::stop_all(){
    analogWrite(FR_SPEED, 0);
    digitalWrite(FR1, LOW);
    digitalWrite(FR2, LOW);
    analogWrite(FL_SPEED, 0);
    digitalWrite(FL1, LOW);
    digitalWrite(FL2, LOW);
    analogWrite(BR_SPEED, 0);
    digitalWrite(BR1, LOW);
    digitalWrite(BR2, LOW);
    analogWrite(BL_SPEED, 0);
    digitalWrite(BL1, LOW);
    digitalWrite(BL2, LOW);  
}
