#pragma once

#include <LiquidCrystal_PCF8574.h>

#include "../system/ActionCreator.hpp"

class ActionManager {
    ActionCreator* creator = nullptr;
    LiquidCrystal_PCF8574* lcd;

    public:
        ActionManager(LiquidCrystal_PCF8574* lcd): lcd(lcd) {}

        bool isUsed() {
            return !!creator;
        }

        bool canAcquire() {
            return !creator;
        }

        void acquire(ActionCreator* creator) {
            if (!canAcquire()) {
                delete creator;
            }

            this->creator = creator;
            this->creator->setup(lcd);
        }

        void update(const RtcDateTime &time, const JoystickActions &action) {
            if (!creator) return;

            ActionCreator* result = creator->update(time, action);
            
            if (result != creator) {
                delete creator;
                creator = nullptr;
            }

            if (!creator && !!result) {
                acquire(result);
            }
        }
};
