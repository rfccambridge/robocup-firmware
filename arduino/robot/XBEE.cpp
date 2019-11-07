#include "Arduino.h"
#include "XBEE.h"

#define XBEE_DEBUG false

// Change + flash per robot (id corresponds to the helmet pattern)
#define ROBOT_ID 8

// Serialization constants - MUST MATCH WITH SOFTWARE
// keys for message verification
#define START_KEY 100
#define END_KEY 255
#define MESSAGE_SIZE 26
// Command value bounds
#define MAX_X 1000
#define MIN_X -1000
#define MAX_Y 1000
#define MIN_Y -1000
#define MAX_W (2 * PI)
#define MIN_W (-2 * PI)

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

// reads and deserializes command, return whether valid command was read
bool XBEE::read_command(Command* cmd) {
    if (Serial5.available()) {
        char buf[BUF_SZ];
        memset(buf, 0, sizeof(buf));
        // Get potential message by reading bytes until matching the END_KEY
        size_t bytes_read = Serial5.readBytesUntil((char) END_KEY, buf, sizeof(buf));
        if (XBEE_DEBUG) print_bytes(&buf[0], sizeof(buf));
        if (bytes_read < MESSAGE_SIZE - 1) {
          if (XBEE_DEBUG) {
            Serial.print("Found END_KEY, but message too short: ");
            Serial.println(bytes_read);
          }
          return false;
        }

        // We put a magic byte START_KEY to start all of our messages.
        // Find the first byte by going back from the END_KEY by the constant message size,
        // and verify that this first magic byte is what we expect
        char *p = buf + bytes_read - MESSAGE_SIZE + 1;
        if (*p != START_KEY) {
          if (XBEE_DEBUG) Serial.println("Expected message start byte != START_KEY");
          return false;
        }
        p += 1; // move past start key to parse actual command
        // The message contains 6 4-byte chunks, each encodes commands for one robot
        // Search through, checking which chunk contains instructions for this robot_id
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
          if (XBEE_DEBUG) Serial.println("Can't find message for this robot!!!");
          return false;
        }

        uint8_t first_byte = (uint8_t) p[0];
        uint8_t x = (uint8_t) p[1];
        uint8_t y = (uint8_t) p[2];
        uint8_t w = (uint8_t) p[3];
        if (XBEE_DEBUG) {
          Serial.println("Here are the 4 bytes for this robot:");
          Serial.println((uint8_t)p[0]);
          Serial.println((uint8_t)p[1]);
          Serial.println((uint8_t)p[2]);
          Serial.println((uint8_t)p[3]);
        }
        cmd->is_dribbling = (first_byte & (1 << 5)) != 0;
        cmd->is_charging = (first_byte & (1 << 6)) != 0;
        cmd->is_kicking = (first_byte & (1 << 7)) != 0;
        cmd->vx = (double) ((((MAX_X - MIN_X) / (double) 256) *  x) + MIN_X);
        cmd->vy = (double) ((((MAX_Y - MIN_Y) / (double) 256) *  y) + MIN_Y);
        cmd->vw = (double) ((((MAX_W - MIN_W) / (double) 256) *  w) + MIN_W);
        if (XBEE_DEBUG) print_command_struct(cmd);
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
