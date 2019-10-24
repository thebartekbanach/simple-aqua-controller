#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"
#include "../../../system/GlobalEventBus.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/ServoValveErrorMessage.hpp"
#include "../../../control/valves/utility.hpp"

#include "../../heaterControl/Events.hpp"

#include "../Settings.hpp"

#include "RefillAquarium.hpp"

#include "../../../control/valves/DisconnectExternalWaterControl.hpp"
#include "messages/AquariumWaterOutletTimeout.hpp"

class RemoveWaterActionCreator: public CommonActionCreator {
    private:
        GlobalEventBus* eventBus;

        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;
        
        WaterChangeModuleSettings* settings;

        Timer waterLevelCheckTimer;
        Timer waterOutletTimeoutTimer;

        ActionCreator* openWaterRemoveValves() {
            return openValvesSynchronusly(valveModule, sewageWaterValve, aquariumWaterValve, this);
        }

        ActionCreator* closeWaterRemoveValves(ActionCreator* nextTarget, bool isError = false) {
            eventBus->send(HEATER_MODULE_ID, HEATER_SAFETY_UNLOCK);

            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, aquariumWaterValve, sewageWaterValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, aquariumWaterValve, sewageWaterValve, nextTarget, nextTarget);
        }

    protected:
        void setup() {
            eventBus->send(HEATER_MODULE_ID, HEATER_SAFETY_LOCK);

            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("   Podmiana wody");
            lcd->setCursor(0, 1);
            lcd->print("   Usuwanie wody");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj");
        }

    public:
        RemoveWaterActionCreator(
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule,
            GlobalEventBus* eventBus,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                eventBus(eventBus),
                settings(settings),
                waterOutletTimeoutTimer(actualTime, settings->aquariumWaterOutflowTimeout * 60) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterOutletTimeoutTimer.isReached(time)) {
                return closeWaterRemoveValves(AquariumWaterOutletTimeout(nullptr), true);
            }

            if (action == BACK) {
                return closeWaterRemoveValves(
                    DisconnectExternalWaterControl("   Podmiana wody", nullptr)
                );
            }

            if (waterLevelCheckTimer.isReached(time)) {
                waterLevelCheckTimer.start(time, 1);

                if (!waterLevelSensor->sense(aquariumWater, changeWaterLevel)) {
                    return closeWaterRemoveValves(
                        new RefillAquariumActionCreator(
                            relayModule,
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