/*#include <iostream>
#include "Order.h"
#include <queue>
// using namespace std;


class OrderbookSide{
    private:
        int side;
        std::priority_queue<Order> orders;
    public:
        OrderbookSide(int side){
            this->side = side;
        };
        void addOrder(Order order){
            orders.push(order);
        }
        Order getTopOrder(){
            return orders.top();
        }
        void fillTopOrder(){
            orders.pop();
        }
        void pfillTopOrder(int qty){
            Order topOrder = orders.top();
            if (qty < topOrder.getQuantity()){
                topOrder.setQuantity(
                    topOrder.getQuantity()-qty);
            }
        }
};*/