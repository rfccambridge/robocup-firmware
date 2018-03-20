#pragma once

#include <xbee.h>
#include <mutex>
#include <cstdint>
#include <stdint.h>

/**
 * @brief Radio with XBEE Series 1
 * 
 * @details This class allows us to communicate with XBEE Series 1 for 
 * robot control. Currently a work in progress. 
 */
class XBEERadio {
    public:
        XBEERadio();
        XBEERadio(std::string usbport);
        ~XBEERadio();


        bool isOpen() const;
        void send(std::string packet);


    protected:
        struct xbee *xbee;
        struct xbee_con *con;
        struct xbee_conAddress address;
        struct xbee_conSettings settings;
        xbee_err ret;

        bool open();

        void command(uint8_t cmd);
        void write(uint8_t reg, uint8_t value);
        uint8_t read(uint8_t reg);

};