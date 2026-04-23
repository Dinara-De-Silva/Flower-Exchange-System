#include "TimeService.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

std::string TimeService::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();    // get the current time point
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000; //extract milliseconds part for .sss in the timestamp
    auto timer = std::chrono::system_clock::to_time_t(now); // convert to time_t for formatting
    std::tm bt;
#ifdef _WIN32
    localtime_s(&bt, &timer); // Windows (MSVC): thread-safe, arguments are reversed vs POSIX
#else
    localtime_r(&timer, &bt); // POSIX (macOS/Linux): thread-safe
#endif

    // 4. Build the string
    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y%m%d-%H%M%S"); // Formats YYYYMMDD-HHMMSS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count(); // Append milliseconds as .sss

    return oss.str();
}
