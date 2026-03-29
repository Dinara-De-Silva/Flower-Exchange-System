#include <iostream>
#include "Order.h"
// using namespace std;

int Order::nextSequence = 0;

Order::Order(std::string clientOrderID, std::string instrument, int side, int quantity, double price){
    this->clientOrderID = clientOrderID;
    this->instrument = instrument;
    this->side = side;
    this->quantity = quantity;
    this->price = price;
    this->sequence = nextSequence++;
    std::cout << "Order created: " << clientOrderID << " " << instrument << " " << side << " " << quantity << " " << price << std::endl;
}
std::string Order::getClientOrderID() const{
     return this->clientOrderID;
}
std::string Order::getInstrument() const{ 
    return this->instrument;
}
int Order::getSide() const{
    return this->side;
}
int Order::getQuantity() const{
    return this->quantity;
}
double Order::getPrice() const{ 
    return this->price;
}
int Order::getSequence() const{
    return this->sequence;
}
bool Order::operator<(const Order& other) const {
    if (this->price != other.getPrice()) {
        return this->price < other.getPrice();
    }
    return this->sequence > other.getSequence();
}
        //based on price, sell order walata honda