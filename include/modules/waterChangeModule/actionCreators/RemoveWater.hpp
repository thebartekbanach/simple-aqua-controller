#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../Settings.hpp"

#include "RefillAquarium.hpp"

#include "messages/ConnectCleanWaterSupply.hpp"
#include "messages/AquariumWaterOutletTimeout.hpp"

class RemoveWaterActionCreator: public CommonActionCreator {
    private:
        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;
        
        WaterChangeModuleSettings* settings;

        Timer waterLevelCheckTimer;
        Timer waterOutletTimeoutTimer;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 1);
            lcd->print("   Podmiana wody");
            lcd->setCursor(0, 2);
            lcd->print("   Usuwanie wody");
        }

    public:
        RemoveWaterActionCreator(
            WaterLevelSensor* waterLevelSensor,
            RelayModule* relayModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                waterLevelSensor(waterLevelSensor),
                relayModule(relayModule),
                settings(settings),
                waterOutletTimeoutTimer(actualTime, settings->aquariumWaterOutflowTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterOutletTimeoutTimer.isReached(time)) {
                relayModule->set(aquariumValve, OFF);
                return AquariumWaterOutletTimeout(nullptr);
            }

            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (!waterLevelSensor->sense(aquariumWater, changeWaterLevel)) {
                    relayModule->set(aquariumValve, OFF);
                    
                    return ConnectCleanWaterSupply(
                        new RefillAquariumActionCreator(
                            waterLevelSensor,
                            relayModule,
                            settings,
                            time
                        )
                    );
                }
            }

            relayModule->set(aquariumValve, ON);
            return this;
        }
};