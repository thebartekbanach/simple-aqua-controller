#pragma once

#include <ThreeWire.h>
#include <RtcDS1302.h>

#include "../utils/log.hpp"

class TimeGuard {
    RtcDS1302<ThreeWire>* rtc;
    RtcDateTime lastProperTime;

    public:
        TimeGuard(RtcDS1302<ThreeWire>* rtc): rtc(rtc) {}

        void initialize() {
            if (!rtc->IsDateTimeValid()) {
                // Common Causes:
                //    1) first time you ran and the device wasn't running yet
                //    2) the battery on the device is low or even missing
                logln("Rtc wasn't running yet or battery level is low");
                setTime(RtcDateTime(__DATE__, "00:00:00"));
            }

            if (rtc->GetIsWriteProtected()) {
                rtc->SetIsWriteProtected(false);
            }

            if (!rtc->GetIsRunning()) {
                logln("Starting rtc");
                rtc->SetIsRunning(true);
            }
        }

        void setTime(const RtcDateTime& time) {
            rtc->SetDateTime(time);
            lastProperTime = time;
        }

        RtcDateTime& getTime() {
            const RtcDateTime time = rtc->GetDateTime();

            auto totalTime = time.TotalSeconds64();
            auto lastTotal = lastProperTime.TotalSeconds64();

            if (!time.IsValid() || totalTime < lastTotal) {
                rtc->SetDateTime(lastProperTime);
                return lastProperTime;
            }

            lastProperTime = time;
            return lastProperTime;
        }
};