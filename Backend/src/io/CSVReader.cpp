// TODO: Read CSV data and do initial verification of data format and valid flower and valid side
#include "io/CSVReader.h"

CSVReader::CSVReader(const std::string& filepath) {
    // TODO: initialize CSV reader with file path and open the file
    this->filepath = filepath;
}

int CSVReader::read() {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Fatal Error: Could not open orders.csv\n";
        return 1;
    }

    std::string currentLine;
    std::getline(file, currentLine); // skip header line
    
    // while (std::getline(file, currentLine)) {

    //     std::stringstream ss(currentLine);
    //     std::string token;
    //     Order tempOrder;
        
    //     std::getline(ss, tempOrder.clientId, ',');
    //     std::getline(ss, tempOrder.instrument, ',');
    //     std::getline(ss, tempOrder.side, ',');

    //     // 6. Type Conversion (Crucial Step!)
    //     // Everything chopped by getline is a std::string. 
    //     // We must manually convert them to numbers.
        
    //     std::getline(ss, token, ',');
    //     tempOrder.quantity = std::stoi(token); // std::stoi = String TO Integer

    //     std::getline(ss, token, ',');
    //     tempOrder.price = std::stod(token);    // std::stod = String TO Double

    //     // 7. Store the parsed order
    //     parsedOrders.push_back(tempOrder);
    // }

    // // 8. Close the file to free up OS resources
    // file.close();

    // // --- Verification Print ---
    // for (const auto& order : parsedOrders) {
    //     std::cout << "Read Order: " << order.clientId << " wants " 
    //               << order.quantity << " " << order.instrument 
    //               << " at $" << order.price << "\n";
    // }

    return 0;
}