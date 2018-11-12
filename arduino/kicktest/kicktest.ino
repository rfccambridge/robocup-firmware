#include "Wire.h"

#define CHARGE 0
#define CHARGE_STOP LOW
#define CHARGE_START HIGH

#define DISCHARGE 4
#define DISCHARGE_ON HIGH
#define DISCHARGE_OFF LOW

#define CHIPENABLE 5
#define CHIPENABLE_ON HIGH
#define CHIPENABLE_OFF LOW

#define DRIBBLER 6
#define DRIBBLER_ON HIGH
#define DRIBBLER_OFF LOW 

#define KICKENABLE 7
#define KICKENABLE_ON HIGH
#define KICKENABLE_OFF LOW

#define INPUT_A_1 3
#define INPUT_B_1 4
#define ENABLE_1 5
#define SPEED_1 10

#define LED 13

/*
 * XBEEs are connected on the PCB to Teensys on Serial 5
 * The connection to the computer is Serial 1
 */

void setup() {
  Serial5.begin(9600);
  Serial.begin(9600);
 
  pinMode(LED, OUTPUT);
  pinMode(CHARGE, OUTPUT);

  pinMode(DISCHARGE, OUTPUT);
  pinMode(CHIPENABLE, OUTPUT);
  pinMode(DRIBBLER, OUTPUT);
  pinMode(KICKENABLE, OUTPUT);  

  
  pinMode(SPEED_1, OUTPUT);

}

void loop() {
  // Disable charging and kick
   digitalWrite(CHARGE, CHARGE_START);
   digitalWrite(KICKENABLE, KICKENABLE_OFF);
   digitalWrite(DISCHARGE, DISCHARGE_OFF);
   digitalWrite(CHIPENABLE, CHIPENABLE_ON);
   digitalWrite(DRIBBLER, DRIBBLER_OFF);

  
   
   digitalWrite(LED, LOW); 

   delay(8000);
   
   digitalWrite(LED, HIGH); 
      digitalWrite(CHARGE, CHARGE_STOP);
   digitalWrite(KICKENABLE, KICKENABLE_ON);
   digitalWrite(KICKENABLE, DISCHARGE_ON);
   delay(2000);


}
