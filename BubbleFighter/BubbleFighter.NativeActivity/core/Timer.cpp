#include "Timer.h"



Timer::Timer(Clock::TimePoint _startingTime, PClock _timerUsed)
	: timerUsed(_timerUsed), 
	startingTime(_startingTime), 
	durationInSeconds(0.0)
{};

Timer::Timer(PClock _timerUsed)
	: timerUsed(_timerUsed), 
	startingTime(_timerUsed->getCurrentTimePoint()), 
	durationInSeconds(0.0)
{};

Timer::~Timer()
{
	timerUsed->removeTimer(this);
};

Timer::operator double() const
{
	return durationInSeconds;
};

Timer::operator float() const
{
	return (float)durationInSeconds;
};

void Timer::reset()
{
	startingTime = timerUsed->getCurrentTimePoint();
	durationInSeconds = 0.0;
};

