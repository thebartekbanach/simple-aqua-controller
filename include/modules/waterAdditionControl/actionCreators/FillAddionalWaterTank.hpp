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
        const ushort& fillingTimeoutTime;

        Timer waterAdditionCheckTimer;
        Timer* fillingTimeout;

        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Trwa dolewanie wody");
            lcd->setCursor(0, 1);
            lcd->print(" wody do zbiornika");
            lcd->setCursor(0, 2);
            lcd->print("   rezerwowego");
            lcd->setCursor(6, 3);
            lcd->write(arrowRightCustomChar);
            lcd->print(" anuluj");
        }

    public:
        FillAddionalWaterTankActionCreator(
            const ushort& fillingTimeoutTime,
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
                fillingTimeout = new Timer(time, fillingTimeoutTime);
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
