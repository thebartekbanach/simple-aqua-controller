#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"

#include "../../../utils/log.hpp"

#include "../Settings.hpp"

#include "messages/AddionalWaterTankRefillComplete.hpp"
#include "messages/AddionalWaterTankRefillCancelled.hpp"
#include "messages/AddionalWaterTankRefillTimeout.hpp"

class FillAddionalWaterTankActionCreator: public CommonActionCreator {
    private:
        const float& fillingTimeoutTime;

        Timer waterAdditionCheckTimer;
        Timer* fillingTimeout = nullptr;

        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;

    protected:
        void setup() {
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
            RelayModule* relayModule):
            fillingTimeoutTime(fillingTimeoutTime),
            waterLevelSensor(waterLevelSensor),
            relayModule(relayModule) {}

        ~FillAddionalWaterTankActionCreator() {
            if (fillingTimeout) delete fillingTimeout;
        }

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (!fillingTimeout) {
                fillingTimeout = new Timer(time, fillingTimeoutTime * 60);
            }

            if (fillingTimeout->isReached(time)) {
                relayModule->set(addionalTankValve, OFF);
                return addionalWaterTankRefillTimeoutMessage();
            }

            if (action == OK) {
                relayModule->set(addionalTankValve, OFF);
                return addionalWaterTankRefillCancelledMessage();
            }

            if (waterAdditionCheckTimer.isReached(time)) {
                waterAdditionCheckTimer.start(time, 1);

                if (waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel)) {
                    relayModule->set(addionalTankValve, OFF);
                    return addionalWaterTankRefillCompleteMessage();
                }
            }

            relayModule->set(addionalTankValve, ON);
            return this;
        }

};
