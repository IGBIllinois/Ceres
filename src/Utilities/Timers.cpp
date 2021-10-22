
#include "Timers.hpp"

using namespace std::chrono;

cIntervalTimer::cIntervalTimer()
	: cIntervalTimer(100)
{
}

cIntervalTimer::cIntervalTimer(uint32_t interval_ms)
{
	mStartTime = std::chrono::high_resolution_clock::now();
	mInterval = std::chrono::milliseconds(interval_ms);
}

uint32_t cIntervalTimer::interval_ms() const
{
	milliseconds ms = duration_cast<milliseconds>(mInterval);
	return ms.count();
}

void cIntervalTimer::interval_ms(uint32_t interval_ms)
{
	mInterval = std::chrono::milliseconds(interval_ms);
}

void cIntervalTimer::reset()
{
	mStartTime = std::chrono::high_resolution_clock::now();
}

bool cIntervalTimer::elapsed()
{
	auto now = std::chrono::high_resolution_clock::now();

	auto diff = now - mStartTime;
	if (diff > mInterval)
	{
		mStartTime = now;
		return true;
	}

	return false;
}
