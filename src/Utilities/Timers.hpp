
#pragma once

#include <chrono>

class cIntervalTimer
{
public:
	cIntervalTimer();
	explicit cIntervalTimer(uint32_t interval_ms);
	~cIntervalTimer() = default;

	bool is_running() const;
	bool is_stopped() const;

	uint32_t interval_sec() const;
	void interval_sec(uint32_t interval_sec);

	uint32_t interval_ms() const;
	void interval_ms(uint32_t interval_ms);

	void start();
	void stop();

	void reset();

	bool elapsed();

private:
	bool mRunning = true;
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::chrono::nanoseconds mInterval;
};


class cOneShotTimer
{
public:
	cOneShotTimer();
	~cOneShotTimer() = default;

	bool is_running() const;
	bool is_stopped() const;

	uint32_t time_sec() const;
	void time_sec(uint32_t interval_sec);

	uint32_t time_ms() const;
	void time_ms(uint32_t interval_ms);

	void start();
	void stop();

	bool elapsed();

private:
	bool mRunning = false;
	std::chrono::high_resolution_clock::time_point mStartTime;
	std::chrono::nanoseconds mInterval;
};
