#ifndef INSTRUMENT_H
#define INSTRUMENT_H
#include "Orderbook.h"
#include <map>
#include <string>

class Instrument {
    private:
        // list of flowers available for trading
        static std::map<std::string, Orderbook*> flowerOrderbooks; // map from flower name to its orderbook

    public:
        // constructor
        Instrument();

        // get the list of flowers available for trading
        static std::vector<std::string> getFlowerList();

        // add a new flower to the list of available flowers
        static void addFlower(std::string flower);
};

#endif