
#pragma once

#include <chrono>

class cIntervalTimer
{
public:
	cIntervalTimer();
	cIntervalTimer(uint32_t interval_ms);
	~cIntervalTimer() = default;

	uint32_t interval_sec() const;
	void interval_sec(uint32_t interval_sec);

	uint32_t interval_ms() const;
	void interval_ms(uint32_t interval_ms);

	void stop();

	void reset();

	bool elapsed();

private:
	bool mRunning = true;
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::chrono::nanoseconds mInterval;
};