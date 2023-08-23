
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

uint32_t cIntervalTimer::interval_sec() const
{
	seconds ms = duration_cast<seconds>(mInterval);
	return ms.count();
}

void cIntervalTimer::interval_sec(uint32_t interval_sec)
{
	mInterval = std::chrono::seconds(interval_sec);
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

void cIntervalTimer::stop()
{
	mRunning = false;
}

void cIntervalTimer::reset()
{
	mRunning = true;
	mStartTime = std::chrono::high_resolution_clock::now();
}

bool cIntervalTimer::elapsed()
{
	if (!mRunning) return false;
	auto now = std::chrono::high_resolution_clock::now();

	auto diff = now - mStartTime;
	if (diff > mInterval)
	{
		mStartTime = now;
		return true;
	}

	return false;
}
