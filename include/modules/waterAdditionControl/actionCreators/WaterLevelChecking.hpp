#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/PreciseTimer.hpp"

#include "../../../control/screen/customCharset.hpp"
#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "../Settings.hpp"

#include "messages/WaterLevelIsEnough.hpp"
#include "messages/AddionalWaterTankLevelIsToLow.hpp"

#include "AddingWater.hpp"

class WaterLevelCheckingActionCreator: public CommonActionCreator {
    const WaterAdditionModuleSettings& settings;

    RelayModule* relayModule;
    WaterLevelSensor* waterLevelSensor;
    GlobalEventBus* eventBus;

    unsigned short numOfAllChecks = 0;
    unsigned short numOfUnsuccessfullChceks = 0;

    unsigned short displayedNumOfAllChecks = 0;

    PreciseTimer checkTimer;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Automatyczna dolewka");
            lcd->setCursor(0, 1);
            lcd->print(" Testowanie poziomu ");
            lcd->setCursor(0, 2);
            lcd->print("  wody: ");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj");
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

            lcd->print(String(numOfAllChecks) + "/" + String(settings.numberOfChecks));

            displayedNumOfAllChecks = numOfAllChecks;
        }

        bool isWaterLevelNotEnought() {
            return numOfUnsuccessfullChceks >=
                (float)settings.numberOfChecks * ((float)settings.minNumberOfUnsuccessfullAttempts / 100);
        }
    
    public:
        WaterLevelCheckingActionCreator(
            const WaterAdditionModuleSettings& settings,
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            GlobalEventBus* eventBus):
                settings(settings),
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                eventBus(eventBus) {}

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (!waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel)) {
                return addionalWaterTankLevelIsToLowMessage();
            }

            if (action == BACK) {
                return nullptr;
            }

            if (checkTimer.done()) {
                numOfAllChecks += 1;
                numOfUnsuccessfullChceks += !waterLevelSensor->sense(aquariumWater, normalWaterLevel);
                checkTimer.start(settings.checkingFrequency);
            }

            if (isWaterLevelNotEnought()) {
                return new AddingWaterActionCreator(
                    settings,
                    relayModule,
                    waterLevelSensor,
                    eventBus
                );
            }

            if (numOfAllChecks >= settings.numberOfChecks) {
                return waterLevelIsEnoughtMessage();
            }

            updateScreenInfo();
            return this;
        }
};