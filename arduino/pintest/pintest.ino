#include <Adafruit_MCP23017.h>

// instantiate IO-expander object
Adafruit_MCP23017 mcp;
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
#define SPEED_1 20

#define LED 13

/*
 * XBEEs are connected on the PCB to Teensys on Serial 5
 * The connection to the computer is Serial 1
 */

void setup() {
  mcp.begin(1);
  Serial5.begin(9600);
  Serial.begin(9600);
 
  pinMode(LED, OUTPUT);
  pinMode(CHARGE, OUTPUT);

  pinMode(DISCHARGE, OUTPUT);
  pinMode(CHIPENABLE, OUTPUT);
  pinMode(DRIBBLER, OUTPUT);
  pinMode(KICKENABLE, OUTPUT);  
  mcp.pinMode(INPUT_A_1,OUTPUT);
  mcp.pinMode(INPUT_B_1,OUTPUT);
  mcp.pinMode(ENABLE_1,OUTPUT);
  mcp.pinMode(1, OUTPUT);
  mcp.pinMode(2, OUTPUT);
  mcp.pinMode(3, OUTPUT);
  mcp.pinMode(4, OUTPUT);
  mcp.pinMode(5, OUTPUT);
  pinMode(SPEED_1, OUTPUT);

}

void loop() {
  // Disable charging and kick
   digitalWrite(CHARGE, CHARGE_STOP);
   digitalWrite(KICKENABLE, KICKENABLE_OFF);
   digitalWrite(DISCHARGE, DISCHARGE_OFF);
   digitalWrite(CHIPENABLE, CHIPENABLE_OFF);
   digitalWrite(DRIBBLER, DRIBBLER_OFF);

   mcp.digitalWrite(1, HIGH);
   mcp.digitalWrite(2, HIGH);
   mcp.digitalWrite(3, HIGH);
   mcp.digitalWrite(4, HIGH);
   mcp.digitalWrite(5, HIGH);
   
   digitalWrite(LED, HIGH); 
   mcp.digitalWrite(ENABLE_1, HIGH);
   mcp.digitalWrite(INPUT_A_1, LOW);
   mcp.digitalWrite(INPUT_B_1, HIGH);
   analogWrite(SPEED_1, 50);
   delay(2000);
   mcp.digitalWrite(ENABLE_1, LOW);
   digitalWrite(LED, LOW);
   delay(2000);
  
  
  if (Serial5.available())
  { // If data comes in from XBee, send it out to serial monitor
    String receivedStr = Serial5.readStringUntil('\n');
    Serial.println(receivedStr);
  }
  

}
