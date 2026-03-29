#ifndef ORDERBOOKSIDE_H
#define ORDERBOOKSIDE_H
#include "Order.h"
#include <queue>
// using namespace std;

template<typename T>
class OrderbookSide{
    private:
        // int side; //may remove this
        std::priority_queue<T> orders;
    public:
        // OrderbookSide(int side){
        //     this->side = side;
        // };
        void addOrder(T order){
            orders.push(order);
        }
        T getTopOrder(){
            return orders.top();
        }
        void fillTopOrder(){
            orders.pop();
        }
        void pfillTopOrder(int qty){
            if (orders.empty() || qty <= 0){
                return;
            }

            T topOrder = orders.top();
            orders.pop();

            if (qty < topOrder.getQuantity()){
                topOrder.setQuantity(
                    topOrder.getQuantity()-qty);
                orders.push(topOrder);
            }
        }
};
#endif

