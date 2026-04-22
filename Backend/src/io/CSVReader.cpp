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
    
    while (std::getline(file, currentLine)) {

        std::vector<std::string> tokens;
        std::string token;

        std::stringstream ss(currentLine);
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) {
            std::cerr << "Error: Invalid line format in CSV: " << currentLine << "\n";
            continue; // skip this line and move to the next
        }

        try {
        // We know we have exactly 5 items, so we can access them by index (0 through 4)
            std::string clientOrderID = tokens[0];
            
            // Convert numbers inside a try block
            int side = std::stoi(tokens[2]);
            int quantity = std::stoi(tokens[3]);
            double price = std::stod(tokens[4]);

            // 5. Make the order
            std::vector<std::string> validFlowers = Exchange::getFlowerList();
            std::string flower = std::find(validFlowers.begin(), validFlowers.end(), tokens[1]) != validFlowers.end() ? tokens[1] : "";
            if (!flower.empty() && (side == 1 || side == 2)) {
                Order order(clientOrderID, flower, side, quantity, price);
                Exchange::getOrderbookQueue(flower)->push(order);
                std::cout << "Order " << order.getOrderID() << " pushed to queue: " << currentLine << "\n";
            } else {
                std::cerr << "Error: Invalid flower or side in line: " << currentLine << "\n";
                return 3; // invalid flower or side
            }

        } catch (const std::exception& e) {
            std::cerr << "Warning: Data conversion error on line: " << currentLine << "\n";
            return 2; // data conversion error
    }
    }

    // 8. Close the file to free up OS resources
    file.close();

    return 0;
}