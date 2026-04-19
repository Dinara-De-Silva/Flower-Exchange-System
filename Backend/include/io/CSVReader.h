#ifndef CSV_READER_H
#define CSV_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Exchange.h"

class CSVReader {
    private:
        std::string filepath;
    public:
        CSVReader(const std::string& filepath);
        int read();    // might change the return type later
};

#endif