#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include "OrderbookSide.h"
#include "Orderbook.h"
#include "ExecutionReport.h"
#include "Order.h"

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
    if (clientOrderID.empty()){
        return -1; // invalid order
    } else if (price <= 0 || quantity <= 0){
        return -2; // invalid price or quantity
    } else if (quantity % 10 != 0){
        return -3; // qty not multiple of 10
    } else if (quantity < 10 || quantity > 10000){
        return -4; // qty out of range
    }
    return 0; // valid order
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
    if (side == 1){ // buy order
        Order* topSellOrder = sellSide->getTopOrder();
        if (topSellOrder == nullptr || topSellOrder->getPrice() > order->getPrice()){
            addOrder(order);
            return 0; // added to book, no execution
            // TODO: execution report (send pfill or fill)
        } else {
            return executeOrder(order);
        }
    } else {    // sell order
        Order* topBuyOrder = buySide->getTopOrder();
        if(topBuyOrder == nullptr || topBuyOrder->getPrice() < order->getPrice()){
            addOrder(order);
            // TODO: execution report (send pfill or fill)
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
                // TODO: add execution print statement
                std::cout << "Executed: " << order->getClientOrderID() << " bought " << newBuyQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newBuyQty < topSellQty){ // new buy fill, top sell pfill
                sellSide->pfillTopOrder(newBuyQty);
                // TODO: add execution print statement
                std::cout << "Executed: " << order->getClientOrderID() << " bought " << newBuyQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newBuyQty > topSellQty){ // new buy again process, top sell fill
                sellSide->fillTopOrder();
                order->setQuantity(newBuyQty - topSellQty);
                std::cout << "Executed: " << order->getClientOrderID() << " bought " << topSellQty << " of " << order->getInstrument() << " at price " << topSellOrder->getPrice() << std::endl;
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
                std::cout << "Executed: " << order->getClientOrderID() << " sold " << newSellQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newSellQty < topBuyQty){ // new sell fill, top buy pfill
                buySide->pfillTopOrder(newSellQty);
                std::cout << "Executed: " << order->getClientOrderID() << " sold " << newSellQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return 1; // fully executed
            } else if (newSellQty > topBuyQty){ // new sell again process - pfill, top buy fill
                buySide->fillTopOrder();
                order->setQuantity(newSellQty - topBuyQty);
                std::cout << "Executed: " << order->getClientOrderID() << " sold " << topBuyQty << " of " << order->getInstrument() << " at price " << topBuyOrder->getPrice() << std::endl;
                return processOrder(order, false); // recursively process the remaining quantity of the new sell order
            }
        } else {
            std::cout << "Undefined behavior: no way new sell come to this block" << std::endl;
            return -1;  // bug, should never come to this block
        }
    }
}

void Orderbook::createExecutionReport(Order* order, std::string status, std::string reasonIn, double executedPriceIn, int executedQuantityIn){
    // this will create an execution report based on the order and the execution result, and write it to a file or database
    // for now, just print the execution report to console
    std::string orderId = order->getOrderID();
    std::string clientOrderId = order->getClientOrderID();
    std::string instrument = order->getInstrument();
    int side = order->getSide();
    double price = order->getPrice();
    int quantity = order->getQuantity();

    std::string transactionTime = ""; // You need to set the transaction time appropriately

    auto now = std::chrono::system_clock::now();    // get the current time point
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000; //extract milliseconds part for .sss in the timestamp
    auto timer = std::chrono::system_clock::to_time_t(now); // convert to time_t for formatting
    std::tm bt;
    localtime_s(&bt, &timer); // convert to local time and store in tm structure for formatting. Note: localtime_s is thread-safe, localtime is not thread-safe. If you are using a different platform, you might need to use localtime_r instead of localtime_s.

    // 4. Build the string
    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y%m%d-%H%M%S"); // Formats YYYYMMDD-HHMMSS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count(); // Appends .sss

    if(status == "rejected"){
        ExecutionReport report(orderId, clientOrderId, instrument, side, price, quantity, status, transactionTime, reason=reasonIn);
    } else if (status == "new" || status == "partially filled" || status == "filled"){
        ExecutionReport report(orderId, clientOrderId, instrument, side, price, quantity, status, transactionTime, executedPrice=executedPriceIn, executedQuantity=executedQuantityIn, reason=reasonIn);
    } else {
        std::cout << "Undefined behavior: invalid status for execution report" << std::endl;
        return; // invalid status, do not create report
    }
    
    report.writeReport();
}