#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order{
    protected:
        std::string orderId;
        std::string clientOrderID;
        std::string instrument;
        int side; // 0 for buy, 1 for sell
        int quantity;
        double price;
        int sequence;

        static int nextSequence;

    public:
    // constructor
        Order(std::string clientOrderID, std::string instrument, int side, int quantity, double price);
    // getters
        std::string getOrderID() const;
        std::string getClientOrderID() const;
        std::string getInstrument() const;
        int getSide() const;
        int getQuantity() const;
        double getPrice() const;
        int getSequence() const;
    // setters
        void setQuantity(int quantity);
    // priority queue comparison operator
        // virtual bool operator<(const Order& other) const;

        virtual ~Order() = default;
};
#endif