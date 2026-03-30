#include "Instrument.h"

Instrument::Instrument(){
    // initialize the flower list with some default flowers
    flowerOrderbooks["Rose"] = new Orderbook();
    flowerOrderbooks["Lavendar"] = new Orderbook();
    flowerOrderbooks["Lotus"] = new Orderbook();
    flowerOrderbooks["Tulip"] = new Orderbook();
    flowerOrderbooks["Orchid"] = new Orderbook();
    std::cout << "Instrument initialized with default flowers." << std::endl;
}

std::vector<std::string> Instrument::getFlowerList(){
    std::vector<std::string> flowerList;
}

void Instrument::addFlower(std::string flower){
    flowerOrderbooks[flower] = new Orderbook();
}