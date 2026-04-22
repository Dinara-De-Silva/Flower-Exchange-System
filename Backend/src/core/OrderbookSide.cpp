#include <iostream>
#include "Order.h"
#include "OrderbookSide.h"
#include <queue>

OrderbookSide::OrderbookSide(int side)
    : orders(OrderPtrComparator(side)) {};

void OrderbookSide::addOrder(Order* order){
    this->orders.push(order);
};

Order* OrderbookSide::getTopOrder(){
    if (this->orders.empty()){
        return nullptr;
    }
    return this->orders.top();
}

void OrderbookSide::fillTopOrder(){
    if (!this->orders.empty()){
        Order* top = this->orders.top();
        this->orders.pop();
        delete top;
    }
}

void OrderbookSide::pfillTopOrder(int qty){
    if (this->orders.empty() || qty <= 0){
        return;
    }

    Order* topOrder = this->orders.top();
    this->orders.pop();

    if (qty < topOrder->getQuantity()){
        topOrder->setQuantity(topOrder->getQuantity()-qty);
        this->orders.push(topOrder);
    }
};