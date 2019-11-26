#include "Kicker.h"
#include "Arduino.h"

#define MOTION_DEBUGGING

#define CHARGE 0
#define CHARGE_STOP LOW
#define CHARGE_START HIGH

#define DISCHARGE 4
#define DISCHARGE_ON HIGH
#define DISCHARGE_OFF LOW

#define CHIPENABLE 5
#define CHIPENABLE_ON HIGH
#define CHIPENABLE_OFF LOW

#define KICKENABLE 7
// #define KICKENABLE_ON HIGH
// #define KICKENABLE_OFF LOW
// TODO: John's current version of the kicker has a pMOSFET.
// We will have a nMOSFET in the future, so right now we use this
// version. The version above will be the final version.
#define KICKENABLE_ON LOW
#define KICKENABLE_OFF HIGH

#define LED 13

Kicker::Kicker() {
}

void Kicker::discharge() {
  
}

void Kicker::kick(double charge_level) {

}

void Kicker::set_charge(double charge_level) {
  
}

void Kicker::force_charge() {
   digitalWrite(CHARGE, CHARGE_START);
   digitalWrite(KICKENABLE, KICKENABLE_OFF);
   digitalWrite(DISCHARGE, DISCHARGE_OFF);
   digitalWrite(LED, LOW); 
}

void Kicker::force_charge_stop() {
   digitalWrite(CHARGE, CHARGE_STOP);
   digitalWrite(KICKENABLE, KICKENABLE_OFF);
   digitalWrite(DISCHARGE, DISCHARGE_OFF);
   digitalWrite(LED, LOW); 
}

void Kicker::force_kick() {
   digitalWrite(LED, HIGH); 
   digitalWrite(CHARGE, CHARGE_STOP);
   digitalWrite(KICKENABLE, KICKENABLE_ON);
}

void Kicker::setup() {
  pinMode(CHARGE, OUTPUT);
  pinMode(DISCHARGE, OUTPUT);
  pinMode(CHIPENABLE, OUTPUT);
  pinMode(KICKENABLE, OUTPUT);  
  force_kick();
}
