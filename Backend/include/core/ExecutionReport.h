#ifndef EXEC_REPORT_H
#define EXEC_REPORT_H

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

class ExecutionReport {
    private:
        std::string orderId;
        std::string clientOrderId;
        std::string instrument;
        int side = 0;
        double price = 0.0;
        int quantity = 0;
        double executedPrice = 0.0;
        int executedQuantity = 0;
        std::string status;
        std::string reason;
        std::string transactionTime;

    public:
        ExecutionReport() = default;
        ExecutionReport(std::string orderId, std::string clientOrderId, std::string instrument, int side, double price, int quantity, std::string status, std::string transactionTime, double executedPrice = 0.0, int executedQuantity = 0, std::string reason = "");

        std::string getOrderId() const { return orderId; }
        std::string getClientOrderId() const { return clientOrderId; }
        std::string getInstrument() const { return instrument; }
        int getSide() const { return side; }
        double getPrice() const { return price; }
        int getQuantity() const { return quantity; }
        double getExecutedPrice() const { return executedPrice; }
        int getExecutedQuantity() const { return executedQuantity; }
        std::string getStatus() const { return status; }
        std::string getReason() const { return reason; }
        std::string getTransactionTime() const { return transactionTime; }
};

// ThreadSafe Execution Report Queue is a Singleton.
class ThreadSafeExecutionReportQueue {
    private:
        static std::queue<ExecutionReport> queue;
        static std::mutex mtx;
        static std::condition_variable cv;
        static bool done; // Flag to tell the writer to stop
    public:
        static void push(const ExecutionReport& report);
        static bool wait_and_pop(ExecutionReport& report);
        static void set_done();
};

#endif