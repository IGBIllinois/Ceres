
#pragma once

#include <chrono>

class cIntervalTimer
{
public:
	cIntervalTimer();
	cIntervalTimer(uint32_t interval_ms);
	~cIntervalTimer() = default;

	uint32_t interval_ms() const;
	void interval_ms(uint32_t interval_ms);

	void reset();

	bool elapsed();

private:
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::chrono::nanoseconds mInterval;
};