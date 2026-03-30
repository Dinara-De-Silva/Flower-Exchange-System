#ifndef EXCHANGE_H
#define EXCHANGE_H
#include "Instrument.h"

class Exchange {
    private:
        bool isRunning;

    public:
        Exchange();
        void start();
        void stop();
        void createOrderbooks();
};

#endif