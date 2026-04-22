#include "CSVWriter.h"

// TODO: Write Execution Reports from the Queue in to the execution_report.csv file.
// If no file exists, create one and write the header line first. If file already exists, append to it without writing the header line again.
#include "CSVWriter.h"

CSVWriter::CSVWriter(const std::string& filepath) {
    this->filepath = filepath;
}

void CSVWriter::writeReport(){
        std::ofstream file(filepath, std::ios::app); // open in append mode
        if (!file.is_open()) {
            std::cerr << "Fatal Error: Could not open execution_report.csv for writing\n";
            return;
        }
    
        // Check if file is empty to write header
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "OrderID,ClientOrderID,Instrument,Side,Price,Quantity,Status,ExecutedPrice,ExecutedQuantity,TransactionTime,Reason\n";
        }
    
        ExecutionReport report;
        while (ThreadSafeExecutionReportQueue::wait_and_pop(report)) {
            file << report.getOrderId() << ","
                << report.getClientOrderId() << ","
                << report.getInstrument() << ","
                << report.getSide() << ","
                << report.getPrice() << ","
                << report.getQuantity() << ","
                << report.getStatus() << ","
                << report.getExecutedPrice() << ","
                << report.getExecutedQuantity() << ","
                << report.getTransactionTime() << ","
                << report.getReason() << "\n";
        }
    
        file.close();
}