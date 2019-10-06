#pragma once

#include "ValveModule.hpp"

#include "../../system/ActionCreator.hpp"

class ConnectExternalWaterControl: public CommonActionCreator {
    private:
        ValveModule* valveModule;
        ActionCreator* nextTarget;
        String title;
        bool isDoneButtonShown = false;

    public:
        ConnectExternalWaterControl(ValveModule* valveModule, String title, ActionCreator* nextTarget):
            title(title), valveModule(valveModule), nextTarget(nextTarget) {}

        void setup() {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print(title);
            lcd->setCursor(0, 1);
            lcd->print(" Podlacz sterowane");
            lcd->setCursor(0, 2);
            lcd->print("zlacze wody i scieku");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj");
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (action == BACK) return nullptr;
            
            const bool areRemotesAvailable = valveModule->areRemotesAvailable();
            
            if (areRemotesAvailable != isDoneButtonShown) {
                isDoneButtonShown = areRemotesAvailable;
                
                lcd->setCursor(12, 3);
                lcd->print(isDoneButtonShown ? "gotowe >" : "        ");
            }

            if (areRemotesAvailable && action == OK) {
                return nextTarget;
            }
            
            return this;
        }
};