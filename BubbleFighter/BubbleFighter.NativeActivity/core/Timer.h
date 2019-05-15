#pragma once

#include <memory>

#include "Clock.h"


class Timer
{
private:
	Clock* const timerUsed;

	Clock::TimePoint startingTime;
	double durationInSeconds;

public:

	Timer(Clock::TimePoint _startingTime, Clock* _timerUsed);
	Timer(Clock* _timerUsed);
	~Timer();

	inline void update(Clock::TimePoint currentTimePoint)
	{
		durationInSeconds = std::chrono::duration<double>(currentTimePoint - startingTime).count();
	};

	inline void update()
	{
		durationInSeconds = std::chrono::duration<double>(timerUsed->getCurrentTimePoint() - startingTime).count();
	};

	operator double() const;

	operator float() const;

	void reset();

};

typedef std::shared_ptr<Timer> PTimer;
