#pragma once

#include <memory>

#include "Clock.h"


class Timer
{
	friend void Clock::update();

private:
	PClock timerUsed;

	Clock::TimePoint startingTime;
	double durationInSeconds;


	inline Clock::TimePoint getCurrentTimepoint()
	{
		if (timerUsed)
			return timerUsed->getCurrentTimePoint();
		return std::chrono::steady_clock::now();
	};

	inline void update(Clock::TimePoint currentTimePoint)
	{
		durationInSeconds = std::chrono::duration<double>(currentTimePoint - startingTime).count();
	};

public:

	Timer()
		: timerUsed(nullptr)
	{
		reset();
	};

	Timer(Clock::TimePoint _startingTime, PClock _timerUsed)
		: timerUsed(_timerUsed),
		startingTime(_startingTime),
		durationInSeconds(0.0)
	{};

	Timer(PClock _timerUsed)
		: timerUsed(_timerUsed),
		startingTime(_timerUsed->getCurrentTimePoint()),
		durationInSeconds(0.0)
	{};

	~Timer()
	{
		timerUsed->removeTimer(this);
	};

	inline void update()
	{
		if (timerUsed != nullptr)
			durationInSeconds = std::chrono::duration<double>(timerUsed->getCurrentTimePoint() - startingTime).count();
		else
		{
			durationInSeconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - startingTime).count();
		}
	};

	operator double() const
	{
		return durationInSeconds;
	};

	operator float() const
	{
		return (float)durationInSeconds;
	};

	void reset()
	{
		startingTime = timerUsed->getCurrentTimePoint();
		durationInSeconds = 0.0;
	};
};

typedef std::shared_ptr<Timer> PTimer;
