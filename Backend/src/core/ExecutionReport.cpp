#include <iostream>
#include "ExecutionReport.h"

ExecutionReport::ExecutionReport(std::string orderId, std::string clientOrderId, std::string instrument, int side, double price, int quantity, std::string status, std::string transactionTime, double executedPrice, int executedQuantity, std::string reason){
    this->orderId = orderId;
    this->clientOrderId = clientOrderId;
    this->instrument = instrument;
    this->side = side;
    this->price = price;
    this->quantity = quantity;
    this->executedPrice = executedPrice;
    this->executedQuantity = executedQuantity;
    this->status = status;
    this->reason = reason;
    this->transactionTime = transactionTime;
}

std::queue<ExecutionReport> ThreadSafeExecutionReportQueue::queue;
std::mutex ThreadSafeExecutionReportQueue::mtx;
std::condition_variable ThreadSafeExecutionReportQueue::cv;
bool ThreadSafeExecutionReportQueue::done = false;

void ThreadSafeExecutionReportQueue::push(const ExecutionReport& report) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(report);
    cv.notify_one(); // Wake up the sleeping writer thread if it's waiting for new reports
}

bool ThreadSafeExecutionReportQueue::wait_and_pop(ExecutionReport& report) {
    std::unique_lock<std::mutex> lock(mtx);
    
    // Wait until the queue has items OR we are done processing
    cv.wait(lock, []() { return !queue.empty() || done; });

    // If we are done and the queue is empty, return false to exit the loop
    if (done && queue.empty()) {
        return false;
    }

    report = queue.front();
    queue.pop();
    return true;
}

void ThreadSafeExecutionReportQueue::set_done() {
    std::lock_guard<std::mutex> lock(mtx);
    done = true;
    cv.notify_all(); // Wake up the writer so it can exit cleanly
}  