
#pragma once

#include <chrono>


/*****************************************************************************
 * 
 * The cDataModel class is the base class for data acquisition.
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

