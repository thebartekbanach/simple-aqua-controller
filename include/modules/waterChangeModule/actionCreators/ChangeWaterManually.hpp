#pragma once

#include "../../../system/ActionCreator.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/valves/ValveModule.hpp"

#include "messages/ServoValvesCloseFailture.hpp"

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

        void updateValvesInfo() {
            lcd->setCursor(9, 1);
            
            lcd->print(
                state == WAITING ?      "oczekiwanie " :
                state == REMOVING ?     "usuwanie    " :
                state == REFILLING ?    "uzupelnianie" :
                "blad zaworu"
            );
        }

        void updateValvesState() {
            switch (state) {
                case WAITING: closeValves(); break;
                case REFILLING: startRefilling(); break;
                case REMOVING: startRemoving(); break;
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

            if (!valveModule->open(sewageWaterValve) || !valveModule->open(aquariumWater)) {
                state = ERROR;
                closeValves();
            }
        }

        void closeValves() {
            bool error = false;

            if (!valveModule->close(sewageWaterValve)) error = true;
            if (!valveModule->close(cleanWaterValve)) error = true;
            if (!valveModule->close(aquariumWaterValve)) error = true;
            if (!valveModule->close(addionalWaterTank)) error = true;

            if (error) state = ERROR;

            updateValvesInfo();
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
        ChangeWaterManuallyActionCreator(ValveModule* valveModule):
                valveModule(valveModule) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (action == OK) {
                closeValves();

                if (state == ERROR) {
                    return ServoValvesCloseFailture(nullptr);
                }

                return nullptr;
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