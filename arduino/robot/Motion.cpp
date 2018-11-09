#include "Motion.h"
#include "Arduino.h"

Motion::Motion(Motor &br_motor, Motor &fr_motor, Motor &bl_motor, Motor &fl_motor) 
               : br(br_motor), fr(fr_motor), bl(bl_motor), fl(fl_motor) {
}

void Motion::setup(){ }
    
void Motion::stop() {
    bl.stop();
    fl.stop();
    br.stop();
    fr.stop(); 
}

void Motion::move(int x, int y, int w){
    bl.turn(x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    fl.turn(-x * sin(THETA) - y * cos(THETA) + ROTATION_SCALE * w);
    fr.turn(-x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
    br.turn(x * sin(THETA) + y * cos(THETA) + ROTATION_SCALE * w);
}
