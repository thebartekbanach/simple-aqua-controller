#pragma once

#include <menu.h>
#include <LiquidCrystal_PCF8574.h>

#include "../control/screen/lcd.hpp"

#include "../control/joystick/JoystickActions.hpp"
#include "../control/joystick/JoystickBind.hpp"

#include "ActionManager.hpp"

enum NavigationManagerState {
    ACTION_MANAGER = 1,
    NAVIGATION = 2,
    BANNER = 3
};

class NavigationManager {
    navRoot* nav;
    ActionManager* actionManager;
    LiquidCrystal_PCF8574* lcd;
    
    NavigationManagerState state = NavigationManagerState::BANNER;

    public:
        NavigationManager(navRoot* nav, ActionManager* actionManager, LiquidCrystal_PCF8574* lcd):
            nav(nav), actionManager(actionManager), lcd(lcd) {}

        void update(const JoystickActions &action) {
            if (actionManager->isUsed() && state != ACTION_MANAGER) {
                nav->idleOn();
                nav->reset();
                state = ACTION_MANAGER;
                return;
            }

            // actionManager has ended its execution | show header only once
            if (!actionManager->isUsed() && state == ACTION_MANAGER) {
                state = BANNER;
                printProgramHeader(lcd);
                return;
            }

            // avoid executing rest of code if it is still ACTION_MANAGER state
            if (state == ACTION_MANAGER) return;

            // if nav is goes sleep, show header only once
            if (state == NAVIGATION && nav->idleTask == nav->sleepTask) {
                state = BANNER;
                printProgramHeader(lcd);
                return;
            }

            // if banner is shown and button action is OK, show navigation menu
            if (state == BANNER && action == OK) {
                state = NAVIGATION;
                nav->idleOff();
                nav->doOutput();
                return;
            }

            if (state == NAVIGATION) {
                nav->doNav(joystickActionToMenuAction(action));
                nav->doOutput();
            }
        }
};