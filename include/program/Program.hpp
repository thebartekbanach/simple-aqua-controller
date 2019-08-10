#pragma once

#include <Arduino.h>

#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/liquidCrystalOut.h>
#include <LiquidCrystal_PCF8574.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <DueFlashStorage.h>

#include "../control/joystick/JoystickController.hpp"
#include "../control/joystick/JoystickBind.hpp"

#include "../control/screen/configuration.hpp"
#include "../control/screen/combineMenuParts.hpp"

#include "../system/System.hpp"

#include "ActionManager.hpp"
#include "NavigationManager.hpp"
#include "TimeGuard.hpp"

#include "../modules/Modules.hpp"

#include "../utils/log.hpp"


class Program {
    chainStream<0> menuInput;
    JoystickController joystick;
    
    ThreeWire rtcWiring;
    RtcDS1302<ThreeWire> rtc;
    DueFlashStorage storage;

    navNode nav_cursors[MAX_DEPTH];
    navRoot nav;

    System system;
    ActionManager actionManager;
    NavigationManager navigationManager;

    TimeGuard timeGuard;

    void buildMenu(Menu::prompt** actionItems, ushort actionsLength, Menu::prompt** settingItems, ushort settingsSize) {
        menuNode* settingsMenu = new menuNode("Ustawienia", settingsSize, (prompt**)settingItems, noAction, noEvent, wrapStyle);

        prompt** mainMenuItems = combineMenuParts(actionItems, actionsLength, (prompt*)settingsMenu);
        menuNode* mainMenu = new menuNode("Menu glowne", actionsLength + 1, mainMenuItems, noAction, noEvent, wrapStyle);

        nav.useMenu(*mainMenu);
        nav.timeOut = 60;
        options = new config('>', '-', defaultNavCodes, true);
    }

    void initializeSystem() {
        logln("System setup...")
        system.setup(&storage);

        logln("Collecting actions and settings")
        MenuItemsResult<Menu::prompt> actions = system.collectActions();
        MenuItemsResult<Menu::prompt> settings = system.collectSettings();

        logln("Starting menu build...")
        buildMenu(actions.items, actions.length, settings.items, settings.length);
    }

    public:
        Program():
            menuInput(nullptr),
            joystick(A0, A1, 150),
            rtcWiring(23, 2, 22),
            rtc(rtcWiring),
            timeGuard(&rtc),
            nav(*(menuNode*)nullptr, nav_cursors, MAX_DEPTH, menuInput, menuOutput),
            actionManager(&lcd),
            navigationManager(&nav, &actionManager, &lcd),
            system(getSystemModules(&nav, &timeGuard), actionManager, rtc) {}

        void setup() {
            logln("Initializing rtc...");
            timeGuard.initialize();

            logln("Initializing lcd...");
            initializeLcd();
            printProgramHeader(&lcd);

            logln("Initializing system...");
            initializeSystem();
        }

        void update() {
            const RtcDateTime time = timeGuard.getTime();
            const JoystickActions jaction = joystick.collectActions();

            actionManager.update(time, jaction);
            system.update(time);
            navigationManager.update(jaction);
        }
};
