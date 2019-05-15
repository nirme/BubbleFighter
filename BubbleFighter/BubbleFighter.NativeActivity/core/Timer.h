#pragma once


#include <list>
#include <algorithm>

#include <chrono>

//#include <time.h>


class Timer
{
public: 
	typedef std::chrono::steady_clock::time_point TimePoint;

private:


	TimePoint timePoint[2];
	unsigned short currentTimePoint;

	double durationInSeconds;

	bool paused;
	std::list<TimeDuration*> durations;

public:

	Timer() : paused(false)
	{
		timePoint[0] = std::chrono::steady_clock::now();

		if (!paused)
			timePoint[1] = std::chrono::steady_clock::now();
		else
			timePoint[1] = timePoint[0];


		currentTimePoint = 1;
		durationInSeconds = std::chrono::duration<double>( timePoint[1] - timePoint[0]).count();
	};


	void update()
	{
		//	struct timespec now;
		//	clock_gettime(CLOCK_MONOTONIC, &now);
		

		if (!paused)
		{
			unsigned short prevTimePont(currentTimePoint);
			currentTimePoint = ++currentTimePoint % (sizeof(timePoint) / sizeof(timePoint[0]));
			durationInSeconds = std::chrono::duration<double>(timePoint[currentTimePoint] - timePoint[prevTimePont]).count();
		}
	};

	//update the counter before the pause to have the most current change in time saved
	void pause()
	{
		update();
		paused = true;
	};

	void unpause()
	{
		timePoint[0] = std::chrono::steady_clock::now();
		timePoint[1] = timePoint[0];
		currentTimePoint = 1;
		durationInSeconds = 0.0;

		paused = true;
	};

	void removeDuration(TimeDuration* td) noexcept
	{
		//find the duration in list and remove it
		auto it = std::find(durations.begin(), durations.end(), td);
		if (it != durations.end())
			durations.erase(it);
	};


};


class TimeDuration
{
private:
	const Timer::TimePoint startingTime;
	double durationInSeconds;

	Timer* const timerUsed;

public:

	TimeDuration(Timer::TimePoint _startingTime, Timer* _timerUsed)
		: startingTime(_startingTime), timerUsed(_timerUsed), durationInSeconds(0.0)
	{};

	~TimeDuration()
	{
		timerUsed->removeDuration(this);
	};

	void update(Timer::TimePoint currentTimePoint)
	{
		durationInSeconds = std::chrono::duration<double>(currentTimePoint - startingTime).count();
	};

	operator double()
	{
		return durationInSeconds;
	};

	operator float()
	{
		return (float) durationInSeconds;
	};
	
};


/**/
