
#include <SoftwareSerial.h>
#include <XBee.h>

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx16 = Rx16Response();
Rx64Response rx64 = Rx64Response();

int dataLED = 13;

uint8_t data = 0;
uint8_t option = 0;

void setup()  {
  Serial.begin(57600);
  xbee.setSerial(Serial);
}

void loop()  {
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    digitalWrite(dataLED, HIGH);

    if (xbee.getResponse().getApiId() == RX_16_RESPONSE ||
        xbee.getResponse().getApiId() == RX_64_RESPONSE) {
      // get a rx packet
      if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
        xbee.getResponse().getRx16Response(rx16);
        data = rx16.getData(0);
        Serial.println(data);
      }
      if (xbee.getResponse().getApiId() == RX_64_RESPONSE) {
        xbee.getResponse().getRx64Response(rx64);
        data = rx64.getData(0);
        Serial.print(data);
      }
    }
  }
  if (xbee.getResponse().isError()) {
    // get the error code
    Serial.print(xbee.getResponse().getErrorCode());
  } 
}


