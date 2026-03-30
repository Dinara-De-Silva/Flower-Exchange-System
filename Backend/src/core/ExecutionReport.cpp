#include <iostream>
#include "ExecutionReport.h"

ExecutionReport::ExecutionReport(std::string orderId, std::string clientOrderId, std::string instrument, int side, double price, int quantity, std::string status, double executedPrice, int executedQuantity, std::string reason, std::string transactionTime){
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

void ExecutionReport::writeReport() const{
    std::cout << "Execution Report - Order ID: " << orderId << ", Client Order ID: " << clientOrderId << ", Instrument: " << instrument << ", Side: " << (side == 1 ? "Buy" : "Sell") << ", Price: " << price << ", Quantity: " << quantity << ", Executed Price: " << executedPrice << ", Executed Quantity: " << executedQuantity << ", Status: " << status << ", Reason: " << reason << ", Transaction Time: " << transactionTime << std::endl;
    // TODO: write to file or database instead of console output
}