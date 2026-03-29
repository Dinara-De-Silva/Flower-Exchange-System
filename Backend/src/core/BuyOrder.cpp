#include "BuyOrder.h"

BuyOrder::BuyOrder(std::string clientOrderID, std::string instrument,int quantity, double price)
    :Order(clientOrderID,instrument,0,quantity,price){};

bool BuyOrder::operator<(const Order& other) const{
    if (this->price != other.getPrice()) {
        return this->price < other.getPrice(); // Higher price = higher priority
    }
    return this->getSequence() > other.getSequence(); // Older order wins on equal price
};
