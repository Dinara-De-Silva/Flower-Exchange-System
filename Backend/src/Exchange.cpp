#include "Exchange.h"
#include <iostream>

Exchange::Exchange() : isRunning(false) {};

void Exchange::start() {
    isRunning = true;
    std::cout << "Exchange started." << std::endl;
    createOrderbooks();
};

void Exchange::stop() {
    isRunning = false;
    std::cout << "Exchange stopped." << std::endl;
};

void Exchange::createOrderbooks() {
    Instrument::Instrument(); // initialize the instrument class to create orderbooks for all instruments
    std::cout << "Orderbooks created for all instruments." << std::endl;

    // TODO: create threads to simulate concurrent order processing for all orderbooks in the map
    Instrument::getFlowerList();
    

}
