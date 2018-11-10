#include "Encoder.h"

#define ENCODER_A 27
#define ENCODER_B 28

Encoder encoder = Encoder(27, 28);
void setup() {
  // put your setup code here, to run once:
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
//  pinMode(13, OUTPUT);     
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);               // wait for a second
//  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);
  Serial.print("HELLO\t"); 
  delay(100);
  int aState = digitalRead(ENCODER_A);
  int bState = digitalRead(ENCODER_B);
  Serial.print(aState);
  Serial.print("\t");
  Serial.println(bState);
}
