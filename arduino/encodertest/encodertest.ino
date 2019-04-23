#define encoder0PinA  25
#define encoder0PinB  26

volatile int encoder0Pos = 0;

void setup() {
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(encoder0PinA, doEncoderA, CHANGE);

  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(encoder0PinB, doEncoderB, CHANGE);

  Serial.begin (9600);
}

void loop() {
  Serial.println(encoder0Pos);
  delay(100);
}

void doEncoderA() {
  encoder0Pos += (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) ? -1 : 1;
}

void doEncoderB() {
   encoder0Pos += (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) ? 1 : -1;
}
