#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"
#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "../../../utils/log.hpp"

#include "messages/WaterLevelIsLow.hpp"
#include "messages/WaterLevelIsEnough.hpp"
#include "messages/WaterAdditionCancelled.hpp"

class WaterLevelCheckingActionCreator: public CommonActionCreator {
    const ushort& numberOfChecks;
    const ushort& checkingFrequency;
    const ushort& minNumOfUnsuccessfullAttemps;

    RelayModule* relayModule;
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
            lcd->print("  wody: ");
            lcd->setCursor(6, 3);
            lcd->write(arrowRightCustomChar);
            lcd->print(" anuluj");
        }

        void updateScreenInfo() {
            if (displayedNumOfAllChecks == numOfAllChecks) return;

            lcd->setCursor(8, 2);

            lcd->write(approvedCustomChar);
            lcd->print((numOfAllChecks > numOfUnsuccessfullChceks
                ? String(numOfAllChecks - numOfUnsuccessfullChceks)
                : String(0)) + " ");

            lcd->write(xCustomChar);
            lcd->print(String(numOfUnsuccessfullChceks) + " ");

            lcd->print(String(numOfAllChecks) + "/" + String(numberOfChecks));

            displayedNumOfAllChecks = numOfAllChecks;
        }

        bool isWaterLevelNotEnought() {
            return numOfUnsuccessfullChceks >= (float)numberOfChecks * ((float)minNumOfUnsuccessfullAttemps / 100);
        }
    
    public:
        WaterLevelCheckingActionCreator(
            const ushort& numberOfChecks,
            const ushort& checkingFrequency,
            const ushort& minNumOfUnsuccessfullAttemps,
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor):
                numberOfChecks(numberOfChecks),
                checkingFrequency(checkingFrequency),
                minNumOfUnsuccessfullAttemps(minNumOfUnsuccessfullAttemps),
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor) {}

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (action == OK) {
                return waterAdditionCancelledMessage(nullptr);
            }

            if (checkTimer.isReached(time)) {
                numOfAllChecks += 1;
                numOfUnsuccessfullChceks += !waterLevelSensor->sense(aquariumWater, normalWaterLevel);
                checkTimer.start(time, checkingFrequency / 1000);
            }

            if (isWaterLevelNotEnought()) {
                return waterLevelIsLowMessage(nullptr);
            }

            if (numOfAllChecks >= numberOfChecks) {
                return waterLevelIsEnoughtMessage(nullptr);
            }

            updateScreenInfo();
            return this;
        }
};