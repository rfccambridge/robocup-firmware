#include "XBEERadio.hpp"
#include <iostream>


int main() {
    XBEERadio* radio = new XBEERadio(); 
    std::string buf;
    for(;;) {
        std::cin >> buf;
        radio->send(buf);
    }    
    return 0;
}