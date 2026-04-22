#ifndef CSVWRITER_H
#define CSVWRITER_H

#include "ExecutionReport.h"
#include <fstream>
#include <iostream>
#include <string>

class CSVWriter {
    private:
        std::string filepath;
    public:
        CSVWriter(const std::string& filepath);
        void writeReport();
};
#endif