#ifndef ORDERBOOKSIDE_H
#define ORDERBOOKSIDE_H
#include "BuyOrder.h"
#include "SellOrder.h"
#include "Order.h"
#include <queue>
// using namespace std;

class OrderbookSide{

    private:
        std::priority_queue<Order*> orders;
    public:
        OrderbookSide(){};
        void addOrder(Order* order);
        Order* getTopOrder();
        void fillTopOrder();
        void pfillTopOrder(int qty);
};

// template<typename T>
// class OrderbookSide{
//     private:
//         // int side; //may remove this
//         std::priority_queue<T> orders;
//     public:
//         // OrderbookSide(int side){
//         //     this->side = side;
//         // };
//         void addOrder(T order){
//             orders.push(order);
//         }
//         T getTopOrder(){
//             return orders.top();
//         }
//         void fillTopOrder(){
//             orders.pop();
//         }
//         void pfillTopOrder(int qty){
//             if (orders.empty() || qty <= 0){
//                 return;
//             }

//             T topOrder = orders.top();
//             orders.pop();

//             if (qty < topOrder.getQuantity()){
//                 topOrder.setQuantity(
//                     topOrder.getQuantity()-qty);
//                 orders.push(topOrder);
//             }
//         }
// };
#endif

