#ifndef ORDERBOOKSIDE_H
#define ORDERBOOKSIDE_H
#include "Order.h"
#include <queue>
#include <vector>
#include <functional>

class OrderbookSide{

    private:
        struct OrderPtrComparator {
            int side;
            OrderPtrComparator(int s) : side(s) {}
            bool operator()(const Order* a, const Order* b) const {
                if (side == 1) {
                    // Buy side: higher price = higher priority
                    if (a->getPrice() != b->getPrice())
                        return a->getPrice() < b->getPrice();
                } else {
                    // Sell side: lower price = higher priority
                    if (a->getPrice() != b->getPrice())
                        return a->getPrice() > b->getPrice();
                }
                // Tiebreak: older order (lower sequence) wins
                return a->getSequence() > b->getSequence();
            }
        };
        std::priority_queue<Order*, std::vector<Order*>, OrderPtrComparator> orders;
    public:
        OrderbookSide(int side);
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

