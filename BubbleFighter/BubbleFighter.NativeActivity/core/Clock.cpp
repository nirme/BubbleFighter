#include "Clock.h"

Clock::Clock() 
	: paused(false)
{
	timePoint[0] = std::chrono::steady_clock::now();

	if (!paused)
		timePoint[1] = std::chrono::steady_clock::now();
	else
		timePoint[1] = timePoint[0];


	currentTimePoint = 1;
	durationInSeconds = std::chrono::duration<double>(timePoint[1] - timePoint[0]).count();
};


void Clock::update()
{
	if (!paused)
	{
		unsigned short prevTimePoint(currentTimePoint);
		currentTimePoint = ++currentTimePoint % (sizeof(timePoint) / sizeof(timePoint[0]));
		timePoint[currentTimePoint] = std::chrono::steady_clock::now();
		durationInSeconds = std::chrono::duration<double>(timePoint[currentTimePoint] - timePoint[prevTimePoint]).count();
	}
};

//update the counter before the pause to have the most current change in time saved
void Clock::pause()
{
	update();
	paused = true;
};

void Clock::unpause()
{
	timePoint[0] = std::chrono::steady_clock::now();
	timePoint[1] = timePoint[0];
	currentTimePoint = 1;
	durationInSeconds = 0.0;

	paused = true;
};

void Clock::removeTimer(Timer* td) noexcept
{
	//find the duration in list and remove it
	auto it = std::find(timers.begin(), timers.end(), td);
	if (it != timers.end())
		timers.erase(it);
};

Clock::TimePoint Clock::getCurrentTimePoint()
{
	return std::chrono::steady_clock::now();
};


PTimer Clock::createTimer()
{
	PTimer pt = std::make_shared<Timer>(this);
	static_assert(std::is_same<decltype(pt), std::shared_ptr<Timer>>::value, "Could not create new timer!");
	timers.push_back(pt.get());
	return pt;
};
