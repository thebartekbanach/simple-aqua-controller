#pragma once

#include "../../../system/ActionCreator.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../Settings.hpp"

#include "RefillAddionalWaterTank.hpp"

#include "messages/AquariumRefillTimeout.hpp"

class RefillAquariumActionCreator: public CommonActionCreator {
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
            lcd->print(" Uzupelnianie wody");
            lcd->setCursor(0, 3);
            lcd->print("    w akwarium");
        }

    public:
        RefillAquariumActionCreator(
            WaterLevelSensor* waterLevelSensor,
            RelayModule* relayModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                waterLevelSensor(waterLevelSensor),
                relayModule(relayModule),
                settings(settings),
                waterRefillTimeoutTimer(actualTime, settings->aquariumRefillTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterRefillTimeoutTimer.isReached(time)) {
                relayModule->set(aquariumValve, OFF);
                return AquariumRefillTimeout(nullptr);
            }

            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (waterLevelSensor->sense(aquariumWater, normalWaterLevel)) {
                    relayModule->set(aquariumValve, OFF);

                    bool addionalWaterTankRefillEnabled = settings->refillAddionalWaterTankDuringWaterChange;
                    bool addionalWaterTankFull = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel);

                    if (addionalWaterTankRefillEnabled && !addionalWaterTankFull) {
                        return new RefillAddionalWaterTankActionCreator(
                            waterLevelSensor,
                            relayModule,
                            settings,
                            time
                        );
                    }

                    return DisconnectCleanWaterSupply(nullptr);
                }
            }

            relayModule->set(aquariumValve, ON);
            return this;
        }
};
