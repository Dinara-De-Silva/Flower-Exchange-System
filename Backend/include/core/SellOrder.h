#ifndef SELLORDER_H
#define SELLORDER_H

#include "Order.h"

class SellOrder : public Order {
    public:
        SellOrder (std :: string clientOrderID, std :: string instrument , int quantity, double price );
        bool operator<(const Order& other) const override;
        ~SellOrder() override = default;
};

#endif