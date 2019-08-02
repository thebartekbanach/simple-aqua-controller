#pragma once

#include <LiquidCrystal_PCF8574.h>

#include "../system/ActionCreator.hpp"

class ActionManager {
    ActionCreator* creator = nullptr;
    LiquidCrystal_PCF8574* lcd;

    bool isAcquiredByMenu = false;

    public:
        ActionManager(LiquidCrystal_PCF8574* lcd): lcd(lcd) {}

        bool isUsedByMenu() {
            return isAcquiredByMenu;
        }

        void acquireByMenu() {
            isAcquiredByMenu = true;
        }

        void menuLeaved() {
            isAcquiredByMenu = false;
        }

        bool isUsedByCreator() {
            return !!creator;
        }

        bool isUsed() {
            return !!creator || isAcquiredByMenu;
        }

        bool canAcquire() {
            return !creator && !isAcquiredByMenu;
        }

        void acquire(ActionCreator* creator) {
            if (isUsedByCreator()) {
                delete creator;
            }

            menuLeaved();

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
