#include "Motion.h"
#include "XBEE.h"

Motion motion;
XBEE xbee(5);

void setup() {
  // put your setup code here, to run once:  
  motion.setup();
  xbee.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  int input[4];
  xbee.read_line(input);
  int id = input[0];
  int* v = &input[1];
  if (id == -1 || id == xbee.id) {
    motion.move(v);
  }
}
