#include "SellOrder.h"

SellOrder::SellOrder(std::string clientOrderID, std::string instrument,int quantity, double price)
    :Order(clientOrderID,instrument,2,quantity,price){};

bool SellOrder::operator<(const Order& other) const{
    if (this->price != other.getPrice()) {
        return this->price > other.getPrice(); // Lower price = higher priority | true -> not top
    }
    return this->getSequence() > other.getSequence(); // Older order wins on equal price | true -> not top
};
