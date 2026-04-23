// CSVReader: validates instrument (must be a known flower) and side (must be 1 or 2).
// All other validation (qty, price) is delegated to Orderbook::verifyOrder on the instrument thread.
#include "io/CSVReader.h"
#include "ExecutionReport.h"
#include "TimeService.h"

CSVReader::CSVReader(const std::string& filepath) {
    this->filepath = filepath;
}

int CSVReader::read() {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Fatal Error: Could not open file: " << filepath << "\n";
        return 1;
    }

    std::string currentLine;
    std::getline(file, currentLine); // skip header line
    int rejectSeq = 0; // local sequence for rejection-only OrderIDs (instrument/side never reach a queue)

    while (std::getline(file, currentLine)) {

        std::vector<std::string> tokens;
        std::string token;

        std::stringstream ss(currentLine);
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() != 5) {
            std::cerr << "Error: Invalid line format in CSV: " << currentLine << "\n";
            continue;
        }

        try {
            std::string clientOrderID = tokens[0];
            std::string instrumentRaw = tokens[1];
            int side = std::stoi(tokens[2]);
            int quantity = std::stoi(tokens[3]);
            double price = std::stod(tokens[4]);

            std::vector<std::string> validFlowers = Exchange::getFlowerList();
            bool validFlower = std::find(validFlowers.begin(), validFlowers.end(), instrumentRaw) != validFlowers.end();
            bool validSide = (side == 1 || side == 2);

            if (!validFlower) {
                std::string orderId = instrumentRaw + "_" + std::to_string(rejectSeq++);
                ExecutionReport report(orderId, clientOrderID, instrumentRaw, side, price, quantity, "rejected", TimeService::getCurrentTimestamp(), 0.0, 0, "Invalid instrument");
                ThreadSafeExecutionReportQueue::push(report);
                std::cerr << "Rejected: Invalid instrument in line: " << currentLine << "\n";
                continue;
            }

            if (!validSide) {
                std::string orderId = instrumentRaw + "_" + std::to_string(rejectSeq++);
                ExecutionReport report(orderId, clientOrderID, instrumentRaw, side, price, quantity, "rejected", TimeService::getCurrentTimestamp(), 0.0, 0, "Invalid side");
                ThreadSafeExecutionReportQueue::push(report);
                std::cerr << "Rejected: Invalid side in line: " << currentLine << "\n";
                continue;
            }

            // Valid instrument and side — push to the instrument's orderbook queue.
            // Qty/price validation is handled by Orderbook::verifyOrder on the instrument thread.
            Order order(clientOrderID, instrumentRaw, side, quantity, price);
            Exchange::getOrderbookQueue(instrumentRaw)->push(order);
            std::cout << "Order " << order.getOrderID() << " pushed to queue: " << currentLine << "\n";

        } catch (const std::exception& e) {
            std::cerr << "Warning: Data conversion error on line: " << currentLine << "\n";
            continue;
        }
    }

    file.close();
    return 0;
}