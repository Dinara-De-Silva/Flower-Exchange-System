#ifndef EXEC_REPORT_H
#define EXEC_REPORT_H

#include <string>

class ExecutionReport {
    private:
        std::string orderId;
        std::string clientOrderId;
        std::string instrument;
        int side;
        double price;
        int quantity;
        double executedPrice;   // can be null
        int executedQuantity;   // can be null
        std::string status;
        std::string reason;     // can be null, if status is rejected, reason will be the rejection reason, if status is partially filled or filled, reason will be the execution reason (e.g. "matched with order xxx")
        std::string transactionTime;

        public:
        ExecutionReport(std::string orderId, std::string clientOrderId, std::string instrument, int side, double price, int quantity, std::string status, std::string transactionTime, double executedPrice = 0.0, int executedQuantity = 0, std::string reason = "");
        void writeReport() const;
};

#endif