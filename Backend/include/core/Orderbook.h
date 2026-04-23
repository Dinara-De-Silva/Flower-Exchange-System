#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <string>
#include "Order.h"

class OrderbookSide;

class Orderbook{
    private:
        OrderbookSide* buySide;
        OrderbookSide* sellSide;

    public:
        Orderbook();
        ~Orderbook();
        int verifyOrder(Order* order);
        void addOrder(Order* order);
        Order* getBuyTopOrder();
        Order* getSellTopOrder();
        int processOrder(Order* order, bool isNewOrder = true);
        int executeOrder(Order* order);
        void createExecutionReport(Order* order, std::string status, std::string reason, double executedPrice, int executedQuantity);

};

#endif