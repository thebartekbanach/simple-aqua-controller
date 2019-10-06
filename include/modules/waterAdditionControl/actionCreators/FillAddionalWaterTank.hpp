#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/ServoValveErrorMessage.hpp"
#include "../../../control/valves/utility.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"

#include "../Settings.hpp"

#include "../../../control/valves/DisconnectExternalWaterControl.hpp"
#include "messages/AddionalWaterTankRefillCancelled.hpp"
#include "messages/AddionalWaterTankRefillTimeout.hpp"

class FillAddionalWaterTankActionCreator: public CommonActionCreator {
    private:
        const float& fillingTimeoutTime;

        Timer waterAdditionCheckTimer;
        Timer* fillingTimeout = nullptr;

        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;
        GlobalEventBus* eventBus;

        ActionCreator* openValves() {
            return openValvesSynchronusly(valveModule, addionalWaterTankValve, cleanWaterValve, this);
        }

        ActionCreator* closeValves(ActionCreator* nextTarget, bool isError = false) {
            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, cleanWaterValve, addionalWaterTankValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, cleanWaterValve, addionalWaterTankValve, nextTarget, nextTarget);
        }

    protected:
        void setup() {
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_ACQUIRE);

            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(" Trwa uzupelnianie");
            lcd->setCursor(0, 1);
            lcd->print("   rezerwy wody");
            lcd->setCursor(10, 3);
            lcd->write(arrowRightCustomChar);
            lcd->print(" anuluj");
        }

    public:
        FillAddionalWaterTankActionCreator(
            const float& fillingTimeoutTime,
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule,
            GlobalEventBus* eventBus):
            fillingTimeoutTime(fillingTimeoutTime),
            waterLevelSensor(waterLevelSensor),
            valveModule(valveModule),
            eventBus(eventBus) {}

        ~FillAddionalWaterTankActionCreator() {
            if (fillingTimeout) delete fillingTimeout;
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_RELEASE);
        }

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (!fillingTimeout) {
                fillingTimeout = new Timer(time, fillingTimeoutTime * 60);
            }

            if (fillingTimeout->isReached(time)) {
                return closeValves(addionalWaterTankRefillTimeoutMessage(), true);
            }

            if (action == OK) {
                return closeValves(DisconnectExternalWaterControl("    Dolewka wody", nullptr));
            }

            if (waterAdditionCheckTimer.isReached(time)) {
                waterAdditionCheckTimer.start(time, 1);

                if (waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel)) {
                    return closeValves(DisconnectExternalWaterControl("    Dolewka wody", nullptr));
                }
            }

            return openValves();
        }

};
