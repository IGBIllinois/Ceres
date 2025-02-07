
#include "Timers.hpp"

using namespace std::chrono;

cIntervalTimer::cIntervalTimer()
	: cIntervalTimer(100)
{
	mRunning = false;
}

cIntervalTimer::cIntervalTimer(uint32_t interval_ms)
{
	mStartTime = std::chrono::high_resolution_clock::now();
	mInterval = std::chrono::milliseconds(interval_ms);
}

bool cIntervalTimer::is_running() const
{
	return mRunning;
}

bool cIntervalTimer::is_stopped() const
{
	return !mRunning;
}

uint32_t cIntervalTimer::interval_sec() const
{
	seconds secs = duration_cast<seconds>(mInterval);
	return secs.count();
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



cOneShotTimer::cOneShotTimer()
{
	mRunning = false;
}

bool cOneShotTimer::is_running() const
{
	return mRunning;
}

bool cOneShotTimer::is_stopped() const
{
	return !mRunning;
}

uint32_t cOneShotTimer::time_sec() const
{
	seconds secs = duration_cast<seconds>(mInterval);
	return secs.count();
}

void cOneShotTimer::time_sec(uint32_t interval_sec)
{
	mInterval = std::chrono::seconds(interval_sec);
}

uint32_t cOneShotTimer::time_ms() const
{
	milliseconds ms = duration_cast<milliseconds>(mInterval);
	return ms.count();
}

void cOneShotTimer::time_ms(uint32_t interval_ms)
{
	mInterval = std::chrono::milliseconds(interval_ms);
}

void cOneShotTimer::start()
{
	mRunning = true;
	mStartTime = std::chrono::high_resolution_clock::now();
}

/*
void cOneShotTimer::stop()
{
	mRunning = false;
}
*/

bool cOneShotTimer::elapsed()
{
	if (!mRunning) return false;
	auto now = std::chrono::high_resolution_clock::now();

	auto diff = now - mStartTime;
	if (diff > mInterval)
	{
		return true;
	}

	return false;
}
