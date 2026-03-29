#ifndef TYPES_H
#define TYPES_H

#include <string>

struct orderRecord {
    std::string clientOrderID;
    std::string instrument;
    int side;
    int quantity;
    double price;
};

#endif
