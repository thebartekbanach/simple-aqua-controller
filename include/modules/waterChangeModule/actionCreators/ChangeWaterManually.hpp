#pragma once

#include "../../../system/ActionCreator.hpp"
#include "../../../system/GlobalEventBus.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/relayModule/RelayModule.hpp"
#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/valves/ValveModule.hpp"

#include "../../../control/valves/actionCreators/LowerThePressure.hpp"
#include "../../../control/valves/actionCreators/DisconnectExternalWaterControl.hpp"

#include "../../heaterControl/Events.hpp"

#include "messages/ServoValvesControlFailture.hpp"

class ChangeWaterManuallyActionCreator: public CommonActionCreator {
    private:
        enum WaterChangeState {
            ERROR = -1,
            REMOVING = 0,
            WAITING = 1,
            REFILLING = 2
        };

        WaterChangeState state = WAITING;

        ValveModule* valveModule;
        RelayModule* relayModule;

        GlobalEventBus* eventBus;

        void updateValvesInfo() {
            lcd->setCursor(8, 1);
            
            lcd->print(
                state == WAITING ?      "oczekiwanie " :
                state == REMOVING ?     "usuwanie    " :
                state == REFILLING ?    "uzupelnianie" :
                "blad zaworow"
            );
        }

        void updateValvesState() {
            switch (state) {
                case WAITING: closeValves(); break;
                case REFILLING: startRefilling(); break;
                case REMOVING: startRemoving(); break;
                default: break;
            }
        }

        void updateValves() {
            updateValvesState();
            updateValvesInfo();
        }

        void startRefilling() {
            closeValves();

            if (state == ERROR) return;

            if (!valveModule->open(aquariumWaterValve) || !valveModule->open(cleanWaterValve)) {
                state = ERROR;
                closeValves();
            }
        }

        void startRemoving() {
            closeValves();

            if (state == ERROR) return;

            eventBus->send(HEATER_MODULE_ID, HEATER_SAFETY_LOCK);

            if (!valveModule->open(aquariumWaterValve) || !valveModule->open(sewageWaterValve)) {
                state = ERROR;
                closeValves();
            }
        }

        void closeValves() {
            bool error = false;

            if (!valveModule->close(sewageWaterValve)) error = true;
            if (!valveModule->close(cleanWaterValve)) error = true;
            if (!valveModule->close(aquariumWaterValve)) error = true;
            if (!valveModule->close(addionalWaterTankValve)) error = true;

            if (error) state = ERROR;
            
            eventBus->send(HEATER_MODULE_ID, HEATER_SAFETY_UNLOCK);
        }

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Reczna podmiana wody");
            lcd->setCursor(0, 1);
            lcd->print("Status: ");
            lcd->setCursor(0, 3);
            lcd->write(doubleVerticalArrowCustomChar);
            lcd->print(" stan    zrobione >");

            updateValves();
        }

    public:
        ChangeWaterManuallyActionCreator(
            RelayModule* relayModule,
            ValveModule* valveModule,
            GlobalEventBus* eventBus):
                relayModule(relayModule),
                valveModule(valveModule),
                eventBus(eventBus) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            relayModule->set(mainPump, state != REFILLING); // too strong water draft when refilling and main pump is turned on

            if (action == OK) {
                closeValves();
                relayModule->set(mainPump, ON);

                if (state == ERROR) {
                    return ServoValvesControlFailture(nullptr);
                }

                return new LowerThePressure(
                    valveModule, "   Podmiana wody", 
                    DisconnectExternalWaterControl("   Podmiana wody", nullptr)
                );
            }

            if (state == ERROR) {
                return this;
            }

            if (action == UP && state != REFILLING) {
                state = (WaterChangeState)(static_cast<int>(state) + 1);
                updateValves();
            }

            if (action == DOWN && state != REMOVING) {
                state = (WaterChangeState)(static_cast<int>(state) - 1);
                updateValves();
            }

            return this;
        }
};