#pragma once

#include "../../../system/ActionCreator.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../Settings.hpp"

#include "messages/AddionalWaterTankRefillTimeout.hpp"
#include "messages/DisconnectCleanWaterSupply.hpp"

class RefillAddionalWaterTankActionCreator: public CommonActionCreator {
    private:
        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;

        WaterChangeModuleSettings* settings;

        Timer waterRefillTimeoutTimer;
        Timer waterLevelCheckTimer;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("   Podmiana wody");
            lcd->setCursor(0, 2);
            lcd->print("Uzupelnianie rezerwy");
        }

    public:
        RefillAddionalWaterTankActionCreator(
            WaterLevelSensor* waterLevelSensor,
            RelayModule* relayModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                waterLevelSensor(waterLevelSensor),
                relayModule(relayModule),
                settings(settings),
                waterRefillTimeoutTimer(actualTime, settings->addionalWaterTankRefillTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterRefillTimeoutTimer.isReached(time)) {
                relayModule->set(addionalTankValve, OFF);
                return AddionalWaterTankRefillTimeout(nullptr);
            }

            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel)) {
                    relayModule->set(addionalTankValve, OFF);
                    return DisconnectCleanWaterSupply(nullptr);
                }
            }

            relayModule->set(addionalTankValve, ON);
            return this;
        }
};
