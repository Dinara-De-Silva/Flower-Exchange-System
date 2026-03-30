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
    localtime_s(&bt, &timer); // convert to local time and store in tm structure for formatting. Note: localtime_s is thread-safe, localtime is not thread-safe. If you are using a different platform, you might need to use localtime_r instead of localtime.

    // 4. Build the string
    std::ostringstream oss;
    oss << std::put_time(&bt, "%Y%m%d-%H%M%S"); // Formats YYYYMMDD-HHMMSS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count(); // Append milliseconds as .sss

    return oss.str();
}
