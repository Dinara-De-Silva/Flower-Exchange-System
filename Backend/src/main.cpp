#include "Exchange.h"
#include <string>

int main(int argc, char* argv[]){
    std::string inputPath = "Backend/data/input/orders.csv";
    std::string outputPath = "Backend/data/output/execution_report.csv";

    if (argc >= 3) {
        inputPath = argv[1];
        outputPath = argv[2];
    }

    Exchange app(inputPath, outputPath);
    app.start();
}