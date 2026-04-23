#ifndef EXCHANGE_H
#define EXCHANGE_H
// #include "Instrument.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include "OrderbookQueue.h"
#include "Order.h"
#include "Orderbook.h"
#include "CSVWriter.h"
#include <thread>

class Exchange {
    private:
        bool isRunning;
        static inline const std::vector<std::string> flowerList = {"Rose", "Lavendar", "Lotus", "Tulip", "Orchid"};
        static std::unordered_map<std::string, std::unique_ptr<Orderbook>> orderbooks;
        static std::unordered_map<std::string, std::unique_ptr<OrderbookQueue<Order, 1024>>> orderbookQueues;
        static std::unordered_map<std::string, std::thread> orderbookThreads;
        std::thread executionReportThread;
        CSVWriter csvWriter;
        std::string inputPath;

        void processOrderbooks(Orderbook& orderbook, OrderbookQueue<Order, 1024>& orderbookQueue);
        void createOrderbookThreads();

    public:
        static std::vector<std::string> getFlowerList() { return flowerList; }
        static std::unique_ptr<OrderbookQueue<Order, 1024>>& getOrderbookQueue(const std::string& flower) { return orderbookQueues[flower]; }
        Exchange(const std::string& inputPath = "Backend/data/input/orders.csv",
                const std::string& outputPath = "Backend/data/output/execution_report.csv");
        void start();
        void stop();
};

#endif