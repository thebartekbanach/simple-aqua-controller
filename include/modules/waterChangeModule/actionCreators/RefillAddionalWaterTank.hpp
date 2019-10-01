#pragma once

#include "../../../system/ActionCreator.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/ServoValveErrorMessage.hpp"
#include "../../../control/valves/utility.hpp"

#include "../Settings.hpp"

#include "messages/AddionalWaterTankRefillTimeout.hpp"
#include "messages/DisconnectCleanWaterSupply.hpp"

class RefillAddionalWaterTankActionCreator: public CommonActionCreator {
    private:
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;

        WaterChangeModuleSettings* settings;

        Timer waterRefillTimeoutTimer;
        Timer waterLevelCheckTimer;
        
        ActionCreator* openRefillValves() {
            return openValvesSynchronusly(valveModule, addionalWaterTankValve, cleanWaterValve, this);
        }

        ActionCreator* closeRefillValves(ActionCreator* nextTarget, bool isError = false) {
            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, cleanWaterValve, addionalWaterTankValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, cleanWaterValve, addionalWaterTankValve, nextTarget, nextTarget);
        }

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
            ValveModule* valveModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                settings(settings),
                waterRefillTimeoutTimer(actualTime, settings->addionalWaterTankRefillTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterRefillTimeoutTimer.isReached(time)) {
                return closeRefillValves(AddionalWaterTankRefillTimeout(nullptr), true);
            }
 
            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel)) {
                    return closeRefillValves(DisconnectCleanWaterSupply(nullptr));
                }
            }

            return openRefillValves();
        }
};
