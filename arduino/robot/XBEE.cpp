#include "Arduino.h"
#include "XBEE.h"

#define XBEE_DEBUG true
// Match with software - keys for message verification
#define START_KEY 100
#define END_KEY 255
#define ROBOT_ID 8 // Change per robot

#define MAX_X 1000
#define MIN_X -1000
#define MAX_Y 1000
#define MIN_Y -1000
#define MAX_W (2 * PI)
#define MIN_W (-2 * PI)

#define MESSAGE_SIZE 26

XBEE::XBEE(int init_id) {
    id = init_id;
}

void XBEE::setup() {
    Serial5.begin(9600);
}

void print_bytes(char* c, int length) {
  for (int i = 0; i < length; i++) {
    Serial.print(*c, HEX);
    Serial.print(", ");
    c += 1;
  }
  Serial.println();
}

bool XBEE::read_line(Command* cmd) {
  
    if (Serial5.available()) {
        char buf[BUF_SZ];
        memset(buf, 0, sizeof(buf));
        size_t bytes_read = Serial5.readBytesUntil((char) END_KEY, buf, sizeof(buf));
        // print_bytes(&buf[0], sizeof(buf));
        if (bytes_read < MESSAGE_SIZE - 1) {
          Serial.println("Message too short");
          Serial.println(bytes_read);
          return false;
        }

        // For the sake of message correctness, we append a magic integer and a comma
        // to the start of all of our messages.
        // Serial.print("wfu\n");
        char *p = buf + bytes_read - MESSAGE_SIZE + 1;
        if (*p != START_KEY) {
          Serial.println("START_KEY incorrect");
          return false;
        }
        p += 1; // move past start key to parse actual command
        // See if command contains instructions for this robot_id
        bool foundMessage = false;
        for (int i = 0; i < 6; i++) {
          if ((15 & ((uint8_t) *p)) == ROBOT_ID) {
            foundMessage = true;
            break;
          }
          p += 4;
        }

        // If no commands for this robot then return
        if (!foundMessage) {
          Serial.println("Can't find message for this robot!!!");
          return false;
        }

        uint8_t first_byte = (uint8_t) p[0];
        uint8_t x = (uint8_t) p[1];
        uint8_t y = (uint8_t) p[2];
        uint8_t w = (uint8_t) p[3];
        /*Serial.println("start");
        Serial.println((uint8_t)p[0]);
        Serial.println((uint8_t)p[1]);
        Serial.println((uint8_t)p[2]);
        Serial.println((uint8_t)p[3]);*/
        cmd->is_dribbling = (first_byte & (1 << 5)) != 0;
        cmd->is_charging = (first_byte & (1 << 6)) != 0;
        cmd->is_kicking = (first_byte & (1 << 7)) != 0;
        cmd->vx = (double) ((((MAX_X - MIN_X) / (double) 256) *  x) + MIN_X);
        cmd->vy = (double) ((((MAX_Y - MIN_Y) / (double) 256) *  y) + MIN_Y);
        cmd->vw = (double) ((((MAX_W - MIN_W) / (double) 256) *  w) + MIN_W);
        print_command_struct(cmd);
        return true;
    } else {
      return false;
    }
}

void print_command_struct(Command* cmd) {
    Serial.print("is_dribbling: ");
    Serial.print(cmd->is_dribbling);
    Serial.print(", is_kicking: ");
    Serial.print(cmd->is_kicking);
    Serial.print(", is_charging: ");
    Serial.print(cmd->is_charging);
    Serial.print(", vx: ");
    Serial.print(cmd->vx);
    Serial.print(", vy: ");
    Serial.print(cmd->vy);
    Serial.print(", vw: ");
    Serial.print(cmd->vw);
    Serial.println("");
}
void XBEE::read_raw(char* inputbuf) {
    if (Serial5.available()) { 
        String receivedStr = Serial5.readStringUntil('\n');
        char buf[BUF_SZ];
        receivedStr.toCharArray(buf, sizeof(buf));
        memcpy(inputbuf, buf, BUF_SZ);
    }
}

void XBEE::write_string(String report) {
  Serial.println("Trying to send report over radio");
  if (Serial5.availableForWrite()) { 
      Serial.println("sending report over radio");
      char test_message[20];
      strncpy("Testing\n", test_message, 20); 
      Serial5.flush();
      Serial5.write(test_message, 20);
      Serial.println("Finished writing");
    }
}
