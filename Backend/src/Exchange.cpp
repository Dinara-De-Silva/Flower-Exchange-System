#include "Exchange.h"
#include "CSVReader.h"
#include <iostream>

// Static member definitions
std::unordered_map<std::string, std::unique_ptr<Orderbook>> Exchange::orderbooks;
std::unordered_map<std::string, std::unique_ptr<OrderbookQueue<Order, 1024>>> Exchange::orderbookQueues;
std::unordered_map<std::string, std::thread> Exchange::orderbookThreads;

Exchange::Exchange(const std::string& inputPath, const std::string& outputPath)
    : isRunning(false), csvWriter(outputPath), inputPath(inputPath) {};

void Exchange::start() {
    isRunning = true;
    std::cout << "Exchange started." << std::endl;
    createOrderbookThreads();
    executionReportThread = std::thread(&CSVWriter::writeReport, &csvWriter);

    // Read orders from CSV
    CSVReader reader(inputPath);
    reader.read();

    // After all orders are read, stop gracefully
    stop();
};

void Exchange::stop() {
    // Signal all queues that no more orders are coming
    for (auto& pair : orderbookQueues) {
        pair.second->setDone();
    }

    // Wait for orderbook threads to drain their queues and exit
    for (auto& pair : orderbookThreads) {
        if (pair.second.joinable()) {
            pair.second.join();
            std::cout << "Thread for " << pair.first << " joined." << std::endl;
        }
    }

    // Now signal the execution report writer to stop and join it
    ThreadSafeExecutionReportQueue::set_done();
    if (executionReportThread.joinable()) {
        executionReportThread.join();
        std::cout << "Execution report thread joined." << std::endl;
    }

    isRunning = false;
    std::cout << "Exchange stopped." << std::endl;
};

void Exchange::createOrderbookThreads() {
    // TODO: create threads to simulate concurrent order processing for all orderbooks in the map
    for (const std::string& flower : Exchange::flowerList) {
        orderbooks[flower] = std::make_unique<Orderbook>();
        orderbookQueues[flower] = std::make_unique<OrderbookQueue<Order, 1024>>();
        orderbookThreads[flower] = std::thread(&Exchange::processOrderbooks, this, std::ref(*orderbooks[flower]), std::ref(*orderbookQueues[flower]));
        std::cout << "Thread created for " << flower << std::endl;
    }
}

void Exchange::processOrderbooks(Orderbook& orderbook, OrderbookQueue<Order, 1024>& orderbookQueue) {
    while (true) {
        if (auto orderOpt = orderbookQueue.pop()) {
            Order* order = new Order(*orderOpt);
            int result = orderbook.processOrder(order);
            // If rejected (negative result), the order was not added to the book or deleted
            // inside executeOrder, so we must free it here.
            if (result < 0) {
                delete order;
            }
        } else if (orderbookQueue.isDone()) {
            break; // No more orders coming and queue is empty
        }
    }
}