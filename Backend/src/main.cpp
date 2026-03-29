#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "core/Types.h"


int main() {
    std::ifstream file("orders.csv");

    if (!file.is_open()) {
        std::cout << "Error opening file\n";
        return 1;
    }

    std::string line;
    orderRecord record;
    // Skip header
    getline(file, line);
    std::string order_record[5];

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        // clientOrderID (string)
        getline(ss, record.clientOrderID, ',');

        // instrument
        getline(ss, record.instrument, ',');

        // side
        getline(ss, token, ',');
        record.side = std::stoi(token);

        // quantity
        getline(ss, token, ',');
        record.quantity = std::stoi(token);

        // price
        getline(ss, token, ',');
        record.price = std::stod(token);

        std::cout << record.clientOrderID << " "
                  << record.instrument << " "
                  << record.side << " "
                  << record.quantity << " "
                  << record.price << std::endl;
    }

    file.close();
}