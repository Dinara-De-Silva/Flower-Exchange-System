#ifndef BUYORDER_H
#define BUYORDER_H

#include "Order.h"

class BuyOrder : public Order {
    public:
        BuyOrder (std::string clientOrderID, std :: string instrument , int quantity, double price );
        bool operator<(const Order& other) const override;
        ~BuyOrder() override = default;
};

#endif