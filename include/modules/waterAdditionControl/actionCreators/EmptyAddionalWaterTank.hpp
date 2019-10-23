#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"
#include "../../../system/GlobalEventBus.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/relayModule/RelayModule.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/ServoValveErrorMessage.hpp"
#include "../../../control/valves/utility.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"

#include "../Events.hpp"
#include "../Settings.hpp"

#include "messages/AddionalWaterTankEmptyingTimeout.hpp"
#include "messages/AddionalWaterTankIsNowEmpty.hpp"

class EmptyAddionalWaterTankActionCreator: public CommonActionCreator {
    private:
        const float& emptyingTimeoutTime;

        Timer waterAdditionCheckTimer;
        Timer* emptyingTimeout = nullptr;

        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;

        GlobalEventBus* eventBus;

        ActionCreator* startEmptying() {
            auto result = openValvesSynchronusly(valveModule, addionalWaterTankValve, sewageWaterValve, this);

            relayModule->set(addionalPump, result == this);

            return result;
        }

        ActionCreator* stopEmptying(ActionCreator* nextTarget, bool isError = false) {
            relayModule->set(addionalPump, OFF);

            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, addionalWaterTankValve, sewageWaterValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, addionalWaterTankValve, sewageWaterValve, nextTarget, nextTarget);
        }

    protected:
        void setup() {
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_ACQUIRE);

            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(" Trwa oproznianie");
            lcd->setCursor(0, 1);
            lcd->print("   rezerwy wody");
            lcd->setCursor(0, 3);
            lcd->print("            anuluj >");
        }

    public:
        EmptyAddionalWaterTankActionCreator(
            const float& emptyingTimeoutTime,
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule,
            GlobalEventBus* eventBus):
                relayModule(relayModule),
                emptyingTimeoutTime(emptyingTimeoutTime),
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                eventBus(eventBus) {}

        ~EmptyAddionalWaterTankActionCreator() {
            if (emptyingTimeout) delete emptyingTimeout;
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_RELEASE);
        }

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (!emptyingTimeout) {
                emptyingTimeout = new Timer(time, emptyingTimeoutTime * 60);
            }

            if (emptyingTimeout->isReached(time)) {
                return stopEmptying(addionalWaterTankEmptyingTimeoutMessage(), true);
            }

            if (action == OK) {
                return stopEmptying(nullptr);
            }

            if (waterAdditionCheckTimer.isReached(time)) {
                waterAdditionCheckTimer.start(time, 1);

                if (!waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel)) {
                    return stopEmptying(addionalWaterTankIsNowEmptyMessage(nullptr));
                }
            }

            return startEmptying();
        }

};
