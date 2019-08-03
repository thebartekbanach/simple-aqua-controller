#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"

#include "../../../utils/log.hpp"

#include "../Settings.hpp"

#include "messages/WaterAdditionComplete.hpp"
#include "messages/WaterAdditionCancelled.hpp"
#include "messages/WaterAdditionTimeout.hpp"
#include "messages/AddionalWaterTankLevelIsToLow.hpp"

class AddingWaterActionCreator: public CommonActionCreator {
    const WaterAdditionModuleSettings& settings;

    RelayModule* relayModule;
    WaterLevelSensor* waterLevelSensor;

    Timer aquariumWaterCheckTimer;
    Timer addionalWaterCheckTimer;
    Timer* waterAdditionTimeout = nullptr;
    WaterLevelSensorDataStream wlsDataStream;

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(" Uzupelnianie wody");
            lcd->setCursor(0, 1);
            lcd->print("   Prosze czekac");
            lcd->setCursor(1, 3);
            lcd->write(arrowRightCustomChar);
            lcd->print(" dokoncz pozniej");
        }
    
    public:
        AddingWaterActionCreator(
            const WaterAdditionModuleSettings& settings,
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor):
                settings(settings),
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                wlsDataStream(5) {}

        ~AddingWaterActionCreator() {
            if (waterAdditionTimeout) delete waterAdditionTimeout;
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (!waterAdditionTimeout) {
                waterAdditionTimeout = new Timer(time, settings.waterAdditionTimeout);
            }

            if (waterAdditionTimeout->isReached(time)) {
                relayModule->set(addionalPump, OFF);
                return waterAdditionTimeoutMessage();
            }

            if (action == OK) {
                relayModule->set(addionalPump, OFF);
                return waterAdditionCancelledMessage();
            }

            if (addionalWaterCheckTimer.isReached(time)) {
                addionalWaterCheckTimer.start(time, 1);
                
                if (!waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel)) {
                    relayModule->set(addionalPump, OFF);
                    return addionalWaterTankLevelIsToLowMessage();
                }
            }

            if (aquariumWaterCheckTimer.isReached(time)) {
                aquariumWaterCheckTimer.start(time, settings.checkingFrequency);

                float average = wlsDataStream.senseAndGetAverage(
                    waterLevelSensor,
                    aquariumWater,
                    normalWaterLevel
                );

                if (average > 0.5) {
                    relayModule->set(addionalPump, OFF);
                    return waterAdditionCompleteMessage();
                }
            }

            relayModule->set(addionalPump, ON);
            return this;
        }
};