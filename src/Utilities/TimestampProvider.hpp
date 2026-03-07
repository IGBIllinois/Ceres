
#pragma once

#include <chrono>


/*****************************************************************************
 * 
 * The cTimestampProvider class provides a common timestamp to all Ceres code.
 * 
 *****************************************************************************/
class cTimestampProvider
{
public:
    cTimestampProvider() = default;
    ~cTimestampProvider() = default;

    static std::uint64_t timestamp_ns();

private:
    static std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
};

