#pragma once

#include <memory>
#include <chrono>


namespace core
{

    class Timer
    {
    protected:
        typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timepoint;

        Timepoint startTime;

        unsigned long updateCount;
        Timepoint currentTime;

    public:
        struct TimeStamp
        {
            unsigned long updateCount;
            Timepoint startTime;
        };


        Timer()
        {};

        void reset()
        {
            startTime = std::chrono::high_resolution_clock::now();
            updateCount = 0;
        };


        void update()
        {
            currentTime = std::chrono::high_resolution_clock::now();
            ++updateCount;
        };


        double timeElapsed(const TimeStamp *stamp = nullptr)
        {
            if (stamp)
            {
                return std::chrono::duration<double>(currentTime - stamp->startTime).count();
            }

            return std::chrono::duration<double>(currentTime - startTime).count();
        };

        unsigned long updatesElapsed(const TimeStamp *stamp = nullptr)
        {
            if (stamp)
            {
                return updateCount - stamp->updateCount;
            }

            return updateCount;
        };

        TimeStamp getTimeStamp()
        {
            return TimeStamp({updateCount, currentTime});
        };




    };

    typedef std::shared_ptr<Timer> TimerPtr;

}
