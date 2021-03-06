#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/PreciseTimer.hpp"
#include "../../../system/Timer.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"

#include "../Settings.hpp"

#include "messages/WaterAdditionComplete.hpp"
#include "messages/WaterAdditionTimeout.hpp"
#include "messages/AddionalWaterTankLevelIsToLow.hpp"

class AddingWaterActionCreator: public CommonActionCreator {
    const WaterAdditionModuleSettings& settings;

    RelayModule* relayModule;
    WaterLevelSensor* waterLevelSensor;
    GlobalEventBus* eventBus;

    PreciseTimer aquariumWaterCheckTimer;
    Timer addionalWaterCheckTimer;
    Timer* waterAdditionTimeout = nullptr;
    WaterLevelSensorDataStream wlsDataStream;

    protected:
        void setup() {
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_ACQUIRE);
            
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(" Uzupelnianie wody");
            lcd->setCursor(0, 1);
            lcd->print("   Prosze czekac");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj");
        }
    
    public:
        AddingWaterActionCreator(
            const WaterAdditionModuleSettings& settings,
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            GlobalEventBus* eventBus):
                settings(settings),
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                eventBus(eventBus),
                wlsDataStream(5) {}

        ~AddingWaterActionCreator() {
            if (waterAdditionTimeout) delete waterAdditionTimeout;
            eventBus->send(WATER_ADDITION_CONTROL_MODULE_ID, WATER_ADDITION_CONTROL_RELEASE);
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (!waterAdditionTimeout) {
                waterAdditionTimeout = new Timer(time, settings.waterAdditionTimeout * 60);
            }

            if (waterAdditionTimeout->isReached(time)) {
                relayModule->set(addionalPump, OFF);
                relayModule->set(mainPump, ON);
                return waterAdditionTimeoutMessage();
            }

            if (action == BACK) {
                relayModule->set(addionalPump, OFF);
                relayModule->set(mainPump, ON);
                return nullptr;
            }

            if (addionalWaterCheckTimer.isReached(time)) {
                addionalWaterCheckTimer.start(time, 1);
                
                if (!waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel)) {
                    relayModule->set(addionalPump, OFF);
                    relayModule->set(mainPump, ON);
                    return addionalWaterTankLevelIsToLowMessage();
                }
            }

            if (aquariumWaterCheckTimer.done()) {
                aquariumWaterCheckTimer.start(settings.checkingFrequency);

                float average = wlsDataStream.senseAndGetAverage(
                    waterLevelSensor,
                    aquariumWater,
                    normalWaterLevel
                );

                if (average > 0.5) {
                    relayModule->set(addionalPump, OFF);
                    relayModule->set(mainPump, ON);
                    return waterAdditionCompleteMessage();
                }
            }

            relayModule->set(mainPump, OFF);
            relayModule->set(addionalPump, ON);
            return this;
        }
};