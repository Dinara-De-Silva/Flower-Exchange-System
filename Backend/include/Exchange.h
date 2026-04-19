#ifndef EXCHANGE_H
#define EXCHANGE_H
// #include "Instrument.h"
#include <unordered_map>
#include "OrderbookQueue.h"
#include "Order.h"
#include "Orderbook.h"
#include <thread>

class Exchange {
    private:
        bool isRunning;
        std::vector<std::string> flowerList = {"Rose", "Lavendar", "Lotus", "Tulip", "Orchid"};
        std::unordered_map<std::string, std::unique_ptr<Orderbook>> orderbooks;
        std::unordered_map<std::string, std::unique_ptr<OrderbookQueue<Order, 1024>>> orderbookQueues;
        std::unordered_map<std::string, std::thread> orderbookThreads;

        void processOrderbooks(Orderbook& orderbook, OrderbookQueue<Order, 1024>& orderbookQueue);
        // void createOrderbooks();
        void createOrderbookThreads();

    public:
        Exchange();
        void start();
        void stop();
};

#endif