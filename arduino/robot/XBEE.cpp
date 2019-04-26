#include "Arduino.h"
#include "XBEE.h"

XBEE::XBEE(int init_id) {
    id = init_id;
}

void XBEE::setup() {
    Serial5.begin(9600);
    // if (XBEE_VERBOSE) {
    //    Serial.begin(9600);
    // }
    // Serial.begin(9600);
}

void XBEE::read_line(int* input) {
    if (Serial5.available()) { 
        String receivedStr = Serial5.readStringUntil('\n');
        char buf[BUF_SZ];
        receivedStr.toCharArray(buf, sizeof(buf));
        char *p = buf;
        // first number is robot id
        input[0] = String(strtok_r(p, ",", &p)).toInt();
        // second number is command type
        input[1] = String(strtok_r(p, ",", &p)).toInt();
        // rest are args
        for (int i = 2; i < 5; i++){
            input[i] = String(strtok_r(p, ",", &p)).toInt();
        }
    }
}

void XBEE::read_raw(char* inputbuf) {
    if (Serial5.available()) { 
        String receivedStr = Serial5.readStringUntil('\n');
        char buf[BUF_SZ];
        receivedStr.toCharArray(buf, sizeof(buf));
        memcpy(inputbuf, buf, BUF_SZ);
    }
}
