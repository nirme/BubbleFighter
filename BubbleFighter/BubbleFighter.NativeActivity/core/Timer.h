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


	inline void update(Clock::TimePoint currentTimePoint)
	{
		durationInSeconds = std::chrono::duration<double>(currentTimePoint - startingTime).count();
	};

public:

	Timer(Clock::TimePoint _startingTime, PClock _timerUsed);
	Timer(PClock _timerUsed);
	~Timer();

	inline void update()
	{
		durationInSeconds = std::chrono::duration<double>(timerUsed->getCurrentTimePoint() - startingTime).count();
	};

	operator double() const;

	operator float() const;

	void reset();

};

typedef std::shared_ptr<Timer> PTimer;
