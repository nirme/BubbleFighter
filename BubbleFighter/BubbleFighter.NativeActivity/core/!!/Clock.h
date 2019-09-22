#pragma once

#include <chrono>

#include <type_traits>
#include <algorithm>
#include <list>

#include "Timer.h"


//#include <time.h>
//	struct timespec now;
//	clock_gettime(CLOCK_MONOTONIC, &now);



class Clock
{
	friend Timer::~Timer();
public:
	typedef std::chrono::steady_clock::time_point TimePoint;

private:


	TimePoint timePoint[2];
	unsigned short currentTimePoint;

	double durationInSeconds;

	bool paused;
	std::list<Timer*> timers;

public:

	Clock();

	void update();

	//update the counter before the pause to have the most current change in time saved
	void pause();
	void unpause();

	void removeTimer(Timer* td) noexcept;

	TimePoint getCurrentTimePoint();
	PTimer createTimer();

};


typedef std::shared_ptr<Clock> PClock;
