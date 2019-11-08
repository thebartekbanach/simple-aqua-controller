#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/Timer.hpp"
#include "../../../system/PreciseTimer.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensorDataStream.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

#include "../../../control/valves/ValveModule.hpp"
#include "../../../control/valves/utility.hpp"

#include "../../../control/valves/actionCreators/ServoValveErrorMessage.hpp"
#include "../../../control/valves/actionCreators/LowerThePressure.hpp"
#include "../../../control/valves/actionCreators/DisconnectExternalWaterControl.hpp"

#include "../Settings.hpp"

#include "RefillAddionalWaterTank.hpp"

#include "messages/AquariumRefillTimeout.hpp"

class RefillAquariumActionCreator: public CommonActionCreator {
    private:
        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;

        WaterLevelSensorDataStream refillAverager;

        WaterChangeModuleSettings* settings;

        Timer waterRefillTimeoutTimer;
        PreciseTimer waterLevelCheckTimer;

        ActionCreator* openAquariumRefillValves() {
            relayModule->set(mainPump, OFF); // too strong water draft
            return openValvesSynchronusly(valveModule, aquariumWaterValve, cleanWaterValve, this);
        }

        ActionCreator* closeAquariumRefillValves(ActionCreator* nextTarget, bool isError = false) {
            relayModule->set(mainPump, ON); // too strong water draft

            if (!isError && nextTarget != nullptr) {
                return closeValvesSynchronusly(valveModule, cleanWaterValve, aquariumWaterValve, nextTarget);
            }

            return closeValvesSynchronusly(valveModule, cleanWaterValve, aquariumWaterValve, nextTarget, nextTarget);
        }

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("   Podmiana wody");
            lcd->setCursor(0, 1);
            lcd->print(" Uzupelnianie wody");
            lcd->setCursor(0, 2);
            lcd->print("    w akwarium");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj");
        }

    public:
        RefillAquariumActionCreator(
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule,
            WaterChangeModuleSettings* settings,
            const RtcDateTime& actualTime):
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                settings(settings),
                waterRefillTimeoutTimer(actualTime, settings->aquariumRefillTimeout * 60),
                refillAverager(5) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (waterRefillTimeoutTimer.isReached(time)) {
                return closeAquariumRefillValves(
                    new LowerThePressure(
                        valveModule, "   Podmiana wody",
                        AquariumRefillTimeout(nullptr)
                    ),
                    true
                );
            }

            if (action == BACK) {
                return closeAquariumRefillValves(
                    new LowerThePressure(
                        valveModule, "   Podmiana wody",
                        DisconnectExternalWaterControl("   Podmiana wody", nullptr)
                    )
                );
            }

            if (waterLevelCheckTimer.done()) {
                waterLevelCheckTimer.start(200);

                refillAverager.senseAndGetAverage(waterLevelSensor, aquariumWater, normalWaterLevel);

                if (refillAverager.getAverage() == 1) {
                    bool addionalWaterTankRefillEnabled = settings->refillAddionalWaterTankDuringWaterChange;
                    bool addionalWaterTankFull = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel);

                    if (addionalWaterTankRefillEnabled && !addionalWaterTankFull) {
                        if (!valveModule->close(aquariumWaterValve)) {
                            return closeAquariumRefillValves(nullptr);
                        }

                        relayModule->set(mainPump, ON);
                        
                        return new RefillAddionalWaterTankActionCreator(
                            waterLevelSensor,
                            valveModule,
                            settings,
                            time
                        );
                    }

                    return closeAquariumRefillValves(
                        new LowerThePressure(
                            valveModule, "   Podmiana wody",
                            DisconnectExternalWaterControl("   Podmiana wody", nullptr)
                        )
                    );
                }
            }

            return openAquariumRefillValves();
        }
};
