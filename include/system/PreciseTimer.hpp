#pragma once

#include <Arduino.h>
#include <limits.h>

class PreciseTimer {
    private:
        bool rolloved = false;
        unsigned long endTime = 0;

    public:
        PreciseTimer() {}
        PreciseTimer(unsigned long howLongMs, unsigned long fireTime = millis()) {
            start(howLongMs, fireTime);
        }

        void start(unsigned long howLongMs, unsigned long fireTime = millis()) {
            endTime = fireTime + howLongMs;
            rolloved = endTime < fireTime;
        }

        unsigned long timeLeft() {
            auto actualTime = millis();
            
            if (rolloved) { // rollover handling
                if (actualTime > endTime) {
                    return ULONG_MAX - actualTime + endTime;
                }
                
                rolloved = false;
            }

            if (actualTime >= endTime) return 0;
            else return endTime - actualTime;
        }

        bool done() {
            return timeLeft() == 0;
        }

        operator bool() {
            return done();
        }
};