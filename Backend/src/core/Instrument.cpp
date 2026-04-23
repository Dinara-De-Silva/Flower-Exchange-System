// #include "Instrument.h"

// Instrument::Instrument(){
//     // initialize the flower list with some default flowers
//     flowerOrderbooks["Rose"] = new Orderbook();
//     flowerOrderbooks["Lavendar"] = new Orderbook();
//     flowerOrderbooks["Lotus"] = new Orderbook();
//     flowerOrderbooks["Tulip"] = new Orderbook();
//     flowerOrderbooks["Orchid"] = new Orderbook();
//     std::cout << "Instrument initialized with default flowers." << std::endl;
// }

// std::vector<std::string> Instrument::getFlowerList(){
//     std::vector<std::string> flowerList;
//     for (const auto& pair : flowerOrderbooks) {
//         flowerList.push_back(pair.first);
//     }
//     return flowerList;
// }

// Orderbook* Instrument::getOrderbook(std::string flower){
//     if (flowerOrderbooks.find(flower) != flowerOrderbooks.end()){
//         return flowerOrderbooks[flower];
//     } else {
//         return nullptr; // flower not found
//     }
// }

// void Instrument::addFlower(std::string flower){
//     flowerOrderbooks[flower] = new Orderbook();
// }