
#include "TimestampProvider.hpp"

// Static Data
std::chrono::time_point<std::chrono::high_resolution_clock> cTimestampProvider::mStartTime = std::chrono::high_resolution_clock::now();

// Static Methods
std::uint64_t cTimestampProvider::timestamp_ns()
{
    const auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - mStartTime).count();
}

