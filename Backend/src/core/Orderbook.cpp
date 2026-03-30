#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include "OrderbookSide.h"
#include "Orderbook.h"
#include "ExecutionReport.h"
#include "Order.h"
#include "TimeService.h"

Orderbook::Orderbook(){
    this->buySide = new OrderbookSide();
    this->sellSide = new OrderbookSide();
};

/*verification error codes:
    -1: empty field(s)
    -2: invalid price or quantity (negative or zero)
    -3: qty not multiple of 10
    -4: qty out of range
     0: valid order
*/
int Orderbook::verifyOrder(Order* order){
    std::string clientOrderID = order->getClientOrderID();
    int quantity = order->getQuantity();
    double price = order->getPrice();
    std::string reason;
    int errorCode = 0;
    if (clientOrderID.empty()){
        reason = "empty field(s)";
        errorCode = -1; // invalid order
    } else if (price <= 0 || quantity <= 0){
        reason = "invalid price or quantity";
        errorCode = -2; // invalid price or quantity
    } else if (quantity % 10 != 0){
        reason = "qty not multiple of 10";
        errorCode = -3; // qty not multiple of 10
    } else if (quantity < 10 || quantity > 10000){
        reason = "qty out of range";
    } else {
        return 0; // valid order
    }
    createExecutionReport(order, "rejected", reason, 0.0, 0); // create execution report for rejected order
    return errorCode;
};

void Orderbook::addOrder(Order* order){
    if (order->getSide() == 1){
        buySide->addOrder(order);
        std::cout<< "Added Buy Order: " << order->getClientOrderID() << " " << order->getInstrument() << " " << order->getQuantity() << " " << order->getPrice() << std::endl;
    } else {
        sellSide->addOrder(order);
        std::cout<< "Added Sell Order: " << order->getClientOrderID() << " " << order->getInstrument() << " " << order->getQuantity() << " " << order->getPrice() << std::endl;
    }
};

Order* Orderbook::getBuyTopOrder(){
    return buySide->getTopOrder();
};

Order* Orderbook::getSellTopOrder(){
    return sellSide->getTopOrder();
};

// process order is called only when a new order is received, not for already existing orders
int Orderbook::processOrder(Order* order, bool isNewOrder){
    // This will handle the matching and execution logic?? have to thing about this more
    // return int for the execution status
    if (isNewOrder){
        int verificationResult = verifyOrder(order);
        if (verificationResult != 0){
            std::cout << "Order verification failed for order " << order->getClientOrderID() << " with error code: " << verificationResult << std::endl;
            return verificationResult; // invalid order, do not process
        }
    }
    int side = order->getSide();
    std::string status = isNewOrder ? "new" : "pfill"; // if it's a new order, status is "new", if it's an existing order being processed again after a partial fill, status is "pfill"
    if (side == 1){ // buy order
        Order* topSellOrder = sellSide->getTopOrder();
        if (topSellOrder == nullptr || topSellOrder->getPrice() > order->getPrice()){
            addOrder(order);
            createExecutionReport(order, status, "order added to buy side", 0.0, 0); // create execution report for new order added to book
            return 0; // added to book, no execution
        } else {
            return executeOrder(order);
        }
    } else {    // sell order
        Order* topBuyOrder = buySide->getTopOrder();
        if(topBuyOrder == nullptr || topBuyOrder->getPrice() < order->getPrice()){
            addOrder(order);
            createExecutionReport(order, status, "order added to sell side", 0.0, 0); // create execution report for new order added to book
            return 0; // added to book, no execution
        } else {
            return executeOrder(order);
        }
    }

};

int Orderbook::executeOrder(Order* order){
    // this will handle fill/pfill methods.
    // with the current logic, only possible scenarios:
    // 1. new buy > top sell -> fill/pfill according to top sell price
    // 2. new sell < top buy -> fill/pfill according to top buy price
    int side = order->getSide();
    if(side == 1){  // new buy order
        Order* topSellOrder = sellSide->getTopOrder();
        if (topSellOrder->getPrice() < order->getPrice()){
            int topSellQty = topSellOrder->getQuantity();
            int newBuyQty = order->getQuantity();
            if (newBuyQty == topSellQty){ //new buy fill, top sell fill
                sellSide->fillTopOrder();
                createExecutionReport(order, "fill", "fully filled against top sell order: " + topSellOrder->getOrderID(), topSellOrder->getPrice(), newBuyQty); // create execution report for fully filled new order
                createExecutionReport(topSellOrder, "fill", "fully filled against new buy order: " + order->getOrderID(), topSellOrder->getPrice(), topSellQty); // create execution report for fully filled top sell order
                std::cout << "Executed: " << order->getOrderID() << " bought " << newBuyQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newBuyQty < topSellQty){ // new buy fill, top sell pfill
                sellSide->pfillTopOrder(newBuyQty);
                createExecutionReport(order, "fill", "fully filled against top sell order: " + topSellOrder->getOrderID(), topSellOrder->getPrice(), newBuyQty); // create execution report for fully filled new order
                createExecutionReport(topSellOrder, "pfill", "partially filled against new buy order: " + order->getOrderID(), topSellOrder->getPrice(), newBuyQty); // create execution report for partially filled top sell order
                std::cout << "Executed: " << order->getOrderID() << " bought " << newBuyQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newBuyQty > topSellQty){ // new buy again process, top sell fill
                sellSide->fillTopOrder();
                order->setQuantity(newBuyQty - topSellQty);
                std::cout << "Executed: " << order->getOrderID() << " bought " << topSellQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
                return processOrder(order, false); // recursively process the remaining quantity of the new buy order
            }
        } else {
            std::cout << "Undefined behavior: no way new buy come to this block" << std::endl;
            return -1;  // bug, should never come to this block
        }
    } else { // new sell order
        Order* topBuyOrder = buySide->getTopOrder();
        if (topBuyOrder->getPrice() > order->getPrice()){
            int topBuyQty = topBuyOrder->getQuantity();
            int newSellQty = order->getQuantity();
            if (newSellQty == topBuyQty){ //new sell fill, top buy fill
                buySide->fillTopOrder();
                createExecutionReport(order, "fill", "fully filled against top buy order: " + topBuyOrder->getOrderID(), topBuyOrder->getPrice(), newSellQty); // create execution report for fully filled new order
                createExecutionReport(topBuyOrder, "fill", "fully filled against new sell order: " + order->getOrderID(), topBuyOrder->getPrice(), topBuyQty); // create execution report for fully filled top buy order
                std::cout << "Executed: " << order->getOrderID() << " sold " << newSellQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newSellQty < topBuyQty){ // new sell fill, top buy pfill
                buySide->pfillTopOrder(newSellQty);
                createExecutionReport(order, "fill", "fully filled against top buy order: " + topBuyOrder->getOrderID(), topBuyOrder->getPrice(), newSellQty); // create execution report for fully filled new order
                createExecutionReport(topBuyOrder, "pfill", "partially filled against new sell order: " + order->getOrderID(), topBuyOrder->getPrice(), newSellQty); // create execution report for partially filled top buy order
                std::cout << "Executed: " << order->getOrderID() << " sold " << newSellQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newSellQty > topBuyQty){ // new sell again process - pfill, top buy fill
                buySide->fillTopOrder();
                order->setQuantity(newSellQty - topBuyQty);
                createExecutionReport(order, "pfill", "partially filled against top buy order: " + topBuyOrder->getOrderID(), topBuyOrder->getPrice(), topBuyQty); // create execution report for partially filled new order
                createExecutionReport(topBuyOrder, "fill", "fully filled against new sell order: " + order->getOrderID(), topBuyOrder->getPrice(), topBuyQty); // create execution report for fully filled top buy order
                std::cout << "Executed: " << order->getOrderID() << " sold " << topBuyQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return processOrder(order, false); // recursively process the remaining quantity of the new sell order
            }
        } else {
            std::cout << "Undefined behavior: no way new sell come to this block" << std::endl;
            return -1;  // bug, should never come to this block
        }
    }
}

void Orderbook::createExecutionReport(Order* order, std::string status, std::string reason, double executedPrice, int executedQuantity){
    // this will create an execution report based on the order and the execution result, and write it to a file or database
    // for now, just print the execution report to console
    std::string orderId = order->getOrderID();
    std::string clientOrderId = order->getClientOrderID();
    std::string instrument = order->getInstrument();
    int side = order->getSide();
    double price = order->getPrice();
    int quantity = order->getQuantity();
    std::string transactionTime = TimeService::getCurrentTimestamp();

    double executedPriceForReport = (status == "rejected") ? 0.0 : executedPrice; // if rejected, executed price is 0
    int executedQuantityForReport = (status == "rejected") ? 0 : executedQuantity; // if rejected, executed quantity is 0

    if (status != "rejected" && status != "pfill" && status != "fill" && status != "new"){
        std::cout << "Undefined behavior: invalid status for execution report" << std::endl;
        return; // invalid status, do not create report
    }
    ExecutionReport report(orderId, clientOrderId, instrument, side, price, quantity, status, executedPriceForReport, executedQuantityForReport, transactionTime, reason);
    report.writeReport();
}