#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H
#include <array>
#include <atomic>
#include <optional>
#include <mutex>
#include <condition_variable>
#include "Order.h"

template<typename T, std::size_t N>
class OrderbookQueue {
    private:
        std::array<T, N> buffer;
        alignas(64) std::atomic<size_t> writeIndex;
        alignas(64) std::atomic<size_t> readIndex;

    public:
        OrderbookQueue() : writeIndex(0), readIndex(0) {}
        
        bool push(const T& item) {
            size_t currentWrite = writeIndex.load(std::memory_order_relaxed);
            // HPC optimization: Bitwise AND replaces modulo if N is a power of 2
            size_t nextWrite = (currentWrite + 1) & (N - 1); 

            if (nextWrite == readIndex.load(std::memory_order_acquire)) {
                return false; // Buffer Full
            }

            buffer[currentWrite] = item;
            writeIndex.store(nextWrite, std::memory_order_release);
            return true;
        }

        std::optional<T> pop() {
            size_t currentRead = readIndex.load(std::memory_order_relaxed);

            if (currentRead == writeIndex.load(std::memory_order_acquire)) {
                return std::nullopt; // Buffer Empty
            }

            T item = buffer[currentRead];
            readIndex.store((currentRead + 1) & (N - 1), std::memory_order_release);
            return item;
        }
};

#endif
