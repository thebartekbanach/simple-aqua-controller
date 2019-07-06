#pragma once

#include <RtcDateTime.h>

class Timer {
    private:
        RtcDateTime* endTime;
    public:
        Timer& start(const RtcDateTime& actualTime, const RtcDateTime& howLong) {
            endTime = new RtcDateTime(actualTime.TotalSeconds64() + howLong.TotalSeconds64());
            return *this;
        }

        Timer& start(const RtcDateTime& end) {
            endTime = new RtcDateTime(end);
            return *this;
        }

        Timer& stop() {
            delete endTime;
            return *this;
        }

        bool isStarted() {
            return !!endTime;
        }

        operator bool() {
            return isStarted();
        }
        
        bool isReached(const RtcDateTime& actualTime) {
            return !endTime ? false : actualTime.TotalSeconds64() > endTime->TotalSeconds64();
        }

        unsigned long long timeToEndInSeconds(const RtcDateTime& actualTime) {
            if (!isStarted() || isReached(actualTime)) return 0;
            return endTime->TotalSeconds64() - actualTime.TotalSeconds64();
        }

        unsigned long long timeToEndInMinutes(const RtcDateTime& actualTime) {
            return timeToEndInSeconds(actualTime) / 60;
        }

        RtcDateTime timeToEnd(const RtcDateTime& actualTime) {
            return RtcDateTime(timeToEndInSeconds(actualTime));
        }
};