#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/ServoValveErrorMessage.hpp"
#include "../../../control/valves/utility.hpp"

#include "../Settings.hpp"

#include "RefillAquarium.hpp"

#include "../../../control/valves/DisconnectExternalWaterControl.hpp"
#include "messages/AquariumWaterOutletTimeout.hpp"

class RemoveWaterActionCreator: public CommonActionCreator {
    private:
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;
        
        WaterChangeModuleSettings* settings;

        Timer waterLevelCheckTimer;
        Timer waterOutletTimeoutTimer;

        ActionCreator* openWaterRemoveValves() {
            return openValvesSynchronusly(valveModule, sewageWaterValve, aquariumWaterValve, this);
        }

        ActionCreator* closeWaterRemoveValves(ActionCreator* nextTarget, bool isError = false) {
            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, aquariumWaterValve, sewageWaterValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, aquariumWaterValve, sewageWaterValve, nextTarget, nextTarget);
        }

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("   Podmiana wody");
            lcd->setCursor(0, 1);
            lcd->print("   Usuwanie wody");
            lcd->setCursor(0, 3);
            lcd->print("            anuluj >");
        }

    public:
        RemoveWaterActionCreator(
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                settings(settings),
                waterOutletTimeoutTimer(actualTime, settings->aquariumWaterOutflowTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterOutletTimeoutTimer.isReached(time)) {
                return closeWaterRemoveValves(AquariumWaterOutletTimeout(nullptr), true);
            }

            if (action == OK) {
                return closeWaterRemoveValves(
                    DisconnectExternalWaterControl("   Podmiana wody", nullptr)
                );
            }

            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (!waterLevelSensor->sense(aquariumWater, changeWaterLevel)) {
                    return closeWaterRemoveValves(
                        new RefillAquariumActionCreator(
                            waterLevelSensor,
                            valveModule,
                            settings,
                            time
                        )
                    );
                }
            }

            return openWaterRemoveValves();
        }
};