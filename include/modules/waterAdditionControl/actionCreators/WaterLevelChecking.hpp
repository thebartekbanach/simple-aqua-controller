#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "../../../utils/log.hpp"

class WaterLevelCheckingActionCreator: public CommonActionCreator {
    const ushort& numberOfChecks;
    const ushort& checkingFrequency;
    const ushort& minNumOfUnsuccessfullAttemps;

    WaterLevelSensor* waterLevelSensor;

    ushort numOfAllChecks = 0;
    ushort numOfUnsuccessfullChceks = 0;

    ushort displayedNumOfAllChecks = 0;

    Timer checkTimer;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Automatyczna dolewka");
            lcd->setCursor(0, 1);
            lcd->print(" Testowanie poziomu ");
            lcd->setCursor(0, 2);
            lcd->print("    wody: ");
            lcd->setCursor(9, 3);
            lcd->write(arrowRightCustomChar);
            lcd->print(" anuluj");
        }

        void updateScreenInfo() {
            if (displayedNumOfAllChecks == numOfAllChecks) return;

            lcd->setCursor(10, 2);
            lcd->print(String(numOfAllChecks) + "/" + String(numberOfChecks));

            displayedNumOfAllChecks = numOfAllChecks;
        }

        bool isWaterLevelNotEnought() {
            return numOfAllChecks > numberOfChecks * (minNumOfUnsuccessfullAttemps / 100);
        }
    
    public:
        WaterLevelCheckingActionCreator(
            const ushort& numberOfChecks,
            const ushort& checkingFrequency,
            const ushort& minNumOfUnsuccessfullAttemps,
            WaterLevelSensor* waterLevelSensor):
                numberOfChecks(numberOfChecks),
                checkingFrequency(checkingFrequency),
                minNumOfUnsuccessfullAttemps(minNumOfUnsuccessfullAttemps),
                waterLevelSensor(waterLevelSensor) {}

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (action == OK) {
                logln("Exit called by user!");
                return nullptr;
            }

            if (checkTimer.isReached(time)) {
                numOfAllChecks += 1;
                numOfUnsuccessfullChceks += !waterLevelSensor->sense(aquariumWater, normalWaterLevel);
                checkTimer.start(time, checkingFrequency);
            }

            if (isWaterLevelNotEnought()) {
                logln("Water level not enough; Starting adding water");
                return nullptr; // AddingWater
            }

            if (numOfAllChecks >= numberOfChecks) {
                logln("Water level enough; Starting info show screen")
                return nullptr; // WaterLevelIsEnought
            }

            updateScreenInfo();
            return this;
        }
};