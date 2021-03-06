#pragma once

#include <RtcDateTime.h>

class Timer {
    private:
        RtcDateTime* endTime = nullptr;

    public:
        Timer(const RtcDateTime& actualTime, const RtcDateTime& howLong) {
            start(actualTime, howLong);
        }

        Timer(const RtcDateTime& actualTime, const unsigned int& howLong) {
            start(actualTime, howLong);
        }

        Timer(const RtcDateTime& endTime) {
            start(endTime);
        }

        Timer() {}

        ~Timer() {
            stop();
        }

        Timer& start(const RtcDateTime& actualTime, const RtcDateTime& howLong) {
            start(actualTime, howLong.TotalSeconds64());
            return *this;
        }

        Timer& start(const RtcDateTime& actualTime, const unsigned int& howLong) {
            endTime = new RtcDateTime(actualTime.TotalSeconds64() + howLong);
            return *this;
        }

        Timer& start(const RtcDateTime& end) {
            endTime = new RtcDateTime(end);
            return *this;
        }

        Timer& stop() {
            if (!!endTime) delete endTime;
            endTime = nullptr;
            return *this;
        }

        bool isStarted() {
            return !!endTime;
        }

        operator bool() {
            return isStarted();
        }
        
        bool isReached(const RtcDateTime& actualTime) {
            return !isStarted() ? true : actualTime.TotalSeconds64() > endTime->TotalSeconds64();
        }

        unsigned int timeToEndInSeconds(const RtcDateTime& actualTime) {
            if (!isStarted() || actualTime.TotalSeconds64() > endTime->TotalSeconds64()) return 0;
            return endTime->TotalSeconds64() - actualTime.TotalSeconds64();
        }

        unsigned int timeToEndInMinutes(const RtcDateTime& actualTime) {
            return timeToEndInSeconds(actualTime) / 60;
        }

        RtcDateTime timeToEnd(const RtcDateTime& actualTime) {
            return RtcDateTime(timeToEndInSeconds(actualTime));
        }
};