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

void Exchange::createOrderbookThreads() {
    // TODO: create threads to simulate concurrent order processing for all orderbooks in the map
    for (const std::string& flower : Exchange::flowerList) {
        orderbooks[flower] = std::make_unique<Orderbook>();
        orderbookQueues[flower] = std::make_unique<OrderbookQueue<Order, 1024>>();
        orderbookThreads[flower] = std::thread(&Exchange::processOrderbooks, this, std::ref(orderbooks[flower]), std::ref(*orderbookQueues[flower]));
        std::cout << "Thread created for" << std::endl;
    }
}

// void Exchange::processOrderbooks(Orderbook& orderbook, OrderbookQueue<Order, 1024>& orderbookQueue) {
//     while (isRunning) {
//         // if (auto orderOpt = orderbookQueue.pop()){

//         }
//     }
        
// }