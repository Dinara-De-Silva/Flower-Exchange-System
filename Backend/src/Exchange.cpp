#include "Exchange.h"
#include <iostream>

Exchange::Exchange() : isRunning(false) {};

void Exchange::start() {
    isRunning = true;
    std::cout << "Exchange started." << std::endl;
    createOrderbookThreads();

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
        std::cout << "Thread created for " << flower << std::endl;
    }
}

void Exchange::processOrderbooks(Orderbook& orderbook, OrderbookQueue<Order, 1024>& orderbookQueue) {
    while (isRunning) {
        // TODO: pop orders from the orderbookQueue and process them using the orderbook's processOrder function
        // then create execution reports based on the processing result and write them to a file or database
        if(auto orderOpt = orderbookQueue.pop()){
            
            Order order = *orderOpt;
            int result = orderbook.processOrder(&order);
        }
    }
        
}