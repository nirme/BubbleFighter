#pragma once


#include <jni.h>
#include <sys/time.h>
#include <chrono>

#include <time.h>


class Timer
{
private:

	typedef std::chrono::steady_clock::time_point TimePoint;

	TimePoint timePoint[2];
	unsigned short currentTimePoint;

	double durationInSeconds;


	Timer()
	{
		timePoint[0] = std::chrono::steady_clock::now();

		currentTimePoint = 1;
		timePoint[currentTimePoint] = std::chrono::steady_clock::now();

		durationInSeconds = std::chrono::duration<double>( timePoint[1] - timePoint[0]).count();
	};


	void update()
	{
		unsigned short prevTimePont(currentTimePoint);
		currentTimePoint = ++currentTimePoint % (sizeof(timePoint) / sizeof(timePoint[0]));
		durationInSeconds = std::chrono::duration<double>(timePoint[currentTimePoint] - timePoint[prevTimePont]).count();
	};

};

/*
int tt()
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	long test = nanoTime();
}

*/
