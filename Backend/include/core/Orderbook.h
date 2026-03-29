#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "OrderbookSide.h"
#include "BuyOrder.h"
#include "SellOrder.h"
#include "Types.h"
#include <iostream>


class Orderbook{
    private:
        OrderbookSide<BuyOrder> buySide;
        OrderbookSide<SellOrder> sellSide;
    public:
        Orderbook();

        void addOrder(orderRecord record){
            if (record.side == 0){
                BuyOrder order(record.clientOrderID, record.instrument, record.quantity, record.price);
                buySide.addOrder(order);
                std::cout<< "Added Buy Order: " << record.clientOrderID << " " << record.instrument << " " << record.quantity << " " << record.price << std::endl;
            } else {
                SellOrder order(record.clientOrderID, record.instrument, record.quantity, record.price);
                sellSide.addOrder(order);
                std::cout<< "Added Sell Order: " << record.clientOrderID << " " << record.instrument << " " << record.quantity << " " << record.price << std::endl;
            }
        }
        Order getTopOrder(int side){
            if (side == 0){
                return buySide.getTopOrder();
            } else {
                return sellSide.getTopOrder();
            }
        }
        void processOrder(orderRecord record){
            
        }
};

#endif