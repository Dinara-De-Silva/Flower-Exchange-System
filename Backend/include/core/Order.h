#ifndef ORDER_H
#define ORDER_H

#include <string>

class Order{
    protected:
        std::string orderId;
        std::string clientOrderID;
        std::string instrument;
        int side = 0; // 1 for buy, 2 for sell
        int quantity = 0;
        double price = 0.0;
        int sequence = 0;

        static int nextSequence;

    public:
    // constructor
        Order() = default;
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

        ~Order() = default;
};
#endif