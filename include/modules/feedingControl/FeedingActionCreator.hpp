#pragma once

#include "../../system/ActionCreator.hpp"
#include "../../system/GlobalEventBus.hpp"
#include "../../system/Timer.hpp"

#include "../../control/screen/customCharset.hpp"

#include "../../utils/log.hpp"

#include "../heaterControl/Events.hpp"

#include "Events.hpp"

class FeedingActionCreator: public CommonActionCreator {
    private:
        Timer timer;
        GlobalEventBus* eventBus;
        uint lastTimeToEnd = 0;

        char getEndForWord(const uint number) {
            if (number < 22) {
                return number == 1 ? 'e'
                    : number > 1 && number < 5 ? 'y'
                    : /* number == 0 || number > 4 */ ' ';
            } else {
                uint lastNumber = number % 10;
                return lastNumber > 1 && lastNumber < 5 ? 'y' : ' ';
            }
        }

        void printTimeLeftToFeedingEnd(const RtcDateTime &time) {
            const uint minutesLeft = timer.timeToEndInMinutes(time) + 1;

            lcd->setCursor(6, 2);

            if (minutesLeft > 1) {
                lastTimeToEnd = minutesLeft;
                lcd->print(String(lastTimeToEnd) + " minut" + getEndForWord(lastTimeToEnd) + "   ");
            } else {
                lastTimeToEnd = timer.timeToEndInSeconds(time);
                lcd->print(String(lastTimeToEnd) + " sekund" + getEndForWord(lastTimeToEnd) + "   ");
            }
        }

        void changeEndTime(const RtcDateTime& actualTime, int addValue) {
            uint timeToEnd = timer.timeToEndInSeconds(actualTime);

            if (addValue < 0 && timeToEnd < (2 * 60)) return;
            if (addValue > 0 && timeToEnd > (59 * 60)) return;

            timer.start(actualTime, timeToEnd + addValue);
        }

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(3, 0);
            lcd->print("Tryb karmienia");
            lcd->setCursor(0, 1);
            lcd->print("Wznowienie pracy za:");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj      czas ");
            lcd->write(doubleVerticalArrowCustomChar);
        }

    public:
        FeedingActionCreator(const RtcDateTime& actualTime, const uint& howLong, GlobalEventBus* eventBus):
            timer(actualTime, howLong),
            eventBus(eventBus) {}

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (timer.isReached(time) || action == OK) {
                eventBus->send(FEEDING_MODULE_ID, ABORT_FEEDING);
                eventBus->send(HEATER_MODULE_ID, HEATER_SAFETY_UNLOCK);
                return nullptr;
            }

            if (action == UP) {
                changeEndTime(time, 60);
            }

            if (action == DOWN) {
                changeEndTime(time, -60);
            }

            printTimeLeftToFeedingEnd(time);
            return this;
        }
};
