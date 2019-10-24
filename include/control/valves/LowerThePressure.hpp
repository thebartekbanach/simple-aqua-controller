#pragma once

#include "ValveModule.hpp"
#include "ServoValveErrorMessage.hpp"

#include "../../system/PreciseTimer.hpp"
#include "../../system/ActionCreator.hpp"

class LowerThePressure: public CommonActionCreator {
    private:
        ValveModule* valveModule;
        ActionCreator* nextTarget;
        String title;

        PreciseTimer openingTimer;

        enum Stages {
            ERROR = -1,
            BEGINNING = 0,
            OPENING = 1,
            CLOSING = 2,
            DONE = 3
        };

        Stages stage = BEGINNING;

        bool closeEveryValve() {
            bool error = false;
            
            if (!valveModule->close(sewageWaterValve)) error = true;
            if (!valveModule->close(cleanWaterValve)) error = true;
            if (!valveModule->close(aquariumWaterValve)) error = true;
            if (!valveModule->close(addionalWaterTankValve)) error = true;

            return !error;
        }

        void openValve() {
            if (closeEveryValve() && valveModule->open(sewageWaterValve)) {
                stage = OPENING;
                openingTimer.start(3000);
            } else stage = ERROR;
        }

        void checkIsFullyOpen() {
            if (openingTimer.done()) stage = CLOSING;
        }

        void closeValve() {
            if (!valveModule->close(sewageWaterValve)) stage = ERROR;
            else stage = DONE;
        }

    public:
        LowerThePressure(ValveModule* valveModule, String title, ActionCreator* nextTarget):
            title(title), valveModule(valveModule), nextTarget(nextTarget) {}

        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(title);
            lcd->setCursor(0, 2);
            lcd->print("Zerowanie cisnienia");
            lcd->setCursor(0, 3);
            lcd->print("   w zlaczu wody");
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (!valveModule->areRemotesAvailable()) {
                return nextTarget;
            }

            switch (stage) {
                case BEGINNING:
                    openValve();
                    break;

                case OPENING:
                    checkIsFullyOpen();
                    break;

                case CLOSING:
                    closeValve();
                    break;

                case DONE:
                    return nextTarget;
                    
                default: // ERROR
                    return servoValveErrorMessage(unknownValve, nextTarget);
            }

            return this;
        }
};