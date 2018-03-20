#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <sstream>
#include <string>
#include <string.h>
#include <cstdint>

#include "XBEERadio.hpp"

static const int Control_Timeout = 1000;

static const char* XBEE_MODE = "xbee1";
static const char* XBEE_DATATYPE = "64-bit Data";
static const char* XBEE_USBPORT = "/dev/ttyUSB1";
static const int NUM_ROBOT_SLOTS = 6;
static const int BAUD_RATE = 9600;



XBEERadio::XBEERadio() {
    // Default on Ubuntu is "/dev/ttyUSB0"
    if ((ret = xbee_setup(&xbee, XBEE_MODE, XBEE_USBPORT, BAUD_RATE)) != XBEE_ENONE) {
        printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
        exit(-1);
    }
    printf("hello\n");
    memset(&address, 0, sizeof(address));
    address.addr64_enabled = 1;
    address.addr64[0] = 0x00;
    address.addr64[1] = 0x00;
    address.addr64[2] = 0x00;
    address.addr64[3] = 0x00;
    address.addr64[4] = 0x00;
    address.addr64[5] = 0x00;
    address.addr64[6] = 0xFF;
    address.addr64[7] = 0xFF;
    printf("hello\n");
    if ((ret = xbee_conNew(xbee, &con, "64-bit Data", &address)) != XBEE_ENONE) {
        xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
        exit(-1);
    }

    xbee_conSettings(con, NULL, &settings);
    settings.disableAck = 1;
    xbee_conSettings(con, &settings, NULL);
}

XBEERadio::XBEERadio(std::string usbport) {
    // Default on Ubuntu is "/dev/ttyUSB0"
    if ((ret = xbee_setup(&xbee, XBEE_MODE, usbport, BAUD_RATE)) != XBEE_ENONE) {
        printf("ret: %d (%s)\n", ret, xbee_errorToStr(ret));
        std::cout<< "[Error]: XBEE not found" << std::endl;
        exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.addr64_enabled = 1;
    address.addr64[0] = 0x00;
    address.addr64[1] = 0x00;
    address.addr64[2] = 0x00;
    address.addr64[3] = 0x00;
    address.addr64[4] = 0x00;
    address.addr64[5] = 0x00;
    address.addr64[6] = 0xFF;
    address.addr64[7] = 0xFF;
    /* 0x000000000000FFFF is the default broadcast address */

    xbee_logLevelSet(xbee, 100); 

    if ((ret = xbee_conNew(xbee, &con, XBEE_DATATYPE, &address)) != XBEE_ENONE) {
        xbee_log(xbee, -1, "xbee_conNew() returned: %d (%s)", ret, xbee_errorToStr(ret));
        std::cout<<"Something went wrong with setting up the connection" << std::endl;
    }

    xbee_conSettings(con, NULL, &settings);
    settings.disableAck = 1;
    xbee_conSettings(con, &settings, NULL);
}


XBEERadio::~XBEERadio() {
    if ((ret = xbee_conEnd(con)) != XBEE_ENONE) {
        xbee_log(xbee, -1, "xbee_conEnd() returned %d", ret);
    }    
    xbee_shutdown(xbee);
}

bool XBEERadio::isOpen() const {
    return true;
}


void XBEERadio::send(std::string packet) {
    xbee_conSettings(con, NULL, &settings);
    settings.disableAck = 1;
    xbee_conSettings(con, &settings, NULL);
    xbee_conTx(con, NULL, "%s\r\n", packet.c_str());
    return;
}



bool XBEERadio::open() {
    return true;
}


void XBEERadio::command(uint8_t cmd) {

}


uint8_t XBEERadio::read(uint8_t reg) {
    return 0;
}





