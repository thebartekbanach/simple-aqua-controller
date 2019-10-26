#pragma once

#include <Arduino.h>
#include <limits.h>

class PreciseTimer {
    private:
        unsigned long fireTime = 0; // need for rollover handle
        unsigned long endTime = 0;

    public:
        PreciseTimer() {}
        PreciseTimer(unsigned long howLongMs) {
            start(howLongMs);
        }

        void start(unsigned long howLongMs) {
            fireTime = millis();
            endTime = fireTime + howLongMs;

            if (endTime < fireTime) {
                fireTime = 0;
            }
        }

        unsigned long timeLeft() {
            auto actualTime = millis();
            
            if (actualTime < fireTime) {
                fireTime = 0;
                endTime = endTime - ULONG_MAX;
            }

            if (actualTime > endTime) return 0;
            else return endTime - actualTime;
        }

        bool done() {
            return timeLeft() == 0;
        }

        operator bool() {
            return done();
        }
};