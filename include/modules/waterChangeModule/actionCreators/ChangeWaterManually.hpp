#pragma once

#include "../../../system/ActionCreator.hpp"

#include "../../../control/screen/customCharset.hpp"

#include "../../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../../control/relayModule/RelayModule.hpp"

class ChangeWaterManuallyActionCreator: public CommonActionCreator {
    private:
        RelayModule* relayModule;

        void updateValveInfo() {
            lcd->setCursor(9, 1);
            
            lcd->print(relayModule->get(aquariumValve)
                ? "otwarty  "
                : "zamkniety"
            );
        }

    protected:
        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Reczna podmiana wody");
            lcd->setCursor(0, 1);
            lcd->print("  Zawor: ");
            lcd->setCursor(0, 3);
            lcd->write(doubleVerticalArrowCustomChar);
            lcd->print(" stan    zrobione >");

            updateValveInfo();
        }

    public:
        ChangeWaterManuallyActionCreator(RelayModule* relayModule):
                relayModule(relayModule) {}

        ActionCreator* update(const RtcDateTime& time, const JoystickActions &action) {
            if (action == OK) {
                relayModule->set(aquariumValve, OFF);
                return nullptr;
            }

            if (action == UP) {
                relayModule->set(aquariumValve, ON);
                updateValveInfo();
            }

            if (action == DOWN) {
                relayModule->set(aquariumValve, OFF);
                updateValveInfo();
            }

            return this;
        }
};