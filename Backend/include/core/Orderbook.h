#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "OrderbookSide.h"
#include "BuyOrder.h"
#include "SellOrder.h"
#include <iostream>

class Orderbook{
    private:
        OrderbookSide* buySide;
        OrderbookSide* sellSide;

    public:
        Orderbook();
        int verifyOrder(Order* order); // this will check if the order is valid or not, return 0 for valid, -1 for invalid
        void addOrder(Order* order);
        Order* getBuyTopOrder();
        Order* getSellTopOrder();
        int processOrder(Order* order, bool isNewOrder = true); // this will handle the matching and execution logic?? have to thing about this more, return int for the execution status
        int executeOrder(Order* order); // This will handle the matching and execution logic?? have to thing about this more
        void createExecutionReport(Order* order, std::string status, std::string reason, double executedPrice, int executedQuantity); // this will create an execution report based on the order and the execution result, and write it to a file or database

};
// class Orderbook{
//     private:
//         OrderbookSide* buySide;
//         OrderbookSide* sellSide;
//     public:
//         Orderbook();

//         void addOrder(orderRecord record){
//             if (record.side == 0){
//                 BuyOrder order(record.clientOrderID, record.instrument, record.quantity, record.price);
//                 buySide.addOrder(order);
//                 std::cout<< "Added Buy Order: " << record.clientOrderID << " " << record.instrument << " " << record.quantity << " " << record.price << std::endl;
//             } else {
//                 SellOrder order(record.clientOrderID, record.instrument, record.quantity, record.price);
//                 sellSide.addOrder(order);
//                 std::cout<< "Added Sell Order: " << record.clientOrderID << " " << record.instrument << " " << record.quantity << " " << record.price << std::endl;
//             }
//         }
//         Order getTopOrder(int side){
//             if (side == 0){
//                 return buySide.getTopOrder();
//             } else {
//                 return sellSide.getTopOrder();
//             }
//         }
//         void processOrder(orderRecord record){
            
//         }
// };

#endif