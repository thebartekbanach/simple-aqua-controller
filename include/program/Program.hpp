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

    RtcDateTime lastProperTime;

    void initializeRtc() {
        if (!rtc.IsDateTimeValid()) {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing
            logln("Rtc wasn't running yet or battery level is low");
            rtc.SetDateTime(RtcDateTime(__DATE__, "00:00:00"));
        }

        if (rtc.GetIsWriteProtected()) {
            rtc.SetIsWriteProtected(false);
        }

        if (!rtc.GetIsRunning()) {
            logln("Starting rtc");
            rtc.SetIsRunning(true);
        }
    }

    void buildMenu(Menu::prompt** actionItems, ushort actionsLength, Menu::menuNode** settingItems, ushort settingsSize) {
        menuNode* settingsMenu = new menuNode("Ustawienia", settingsSize, (prompt**)settingItems, noAction, noEvent, wrapStyle);

        prompt** mainMenuItems = combineMenuParts(actionItems, actionsLength, (prompt*)settingsMenu);
        menuNode* mainMenu = new menuNode("Menu glowne", actionsLength + 1, mainMenuItems, noAction, noEvent, wrapStyle);

        nav.useMenu(*mainMenu);
        nav.timeOut = 60;
    }

    void initializeSystem() {
        logln("System setup...")
        system.setup(&storage);

        logln("Collecting actions and settings")
        MenuItemsResult<Menu::prompt> actions = system.collectActions();
        MenuItemsResult<Menu::menuNode> settings = system.collectSettings();

        logln("Starting menu build...")
        buildMenu(actions.items, actions.length, settings.items, settings.length);
    }

    RtcDateTime& getTime() {
        const RtcDateTime time = rtc.GetDateTime();

        auto totalTime = time.TotalSeconds64();
        auto lastTotal = lastProperTime.TotalSeconds64();

        if (!time.IsValid() || totalTime < lastTotal) {
            rtc.SetDateTime(lastProperTime);
            return lastProperTime;
        }

        lastProperTime = time;
        return lastProperTime;
    }

    public:
        Program():
            menuInput(nullptr),
            joystick(A0, A1, 150),
            rtcWiring(23, 2, 22),
            rtc(rtcWiring),
            nav(*(menuNode*)nullptr, nav_cursors, MAX_DEPTH, menuInput, menuOutput),
            actionManager(&lcd),
            navigationManager(&nav, &actionManager, &lcd),
            system(getSystemModules(), actionManager, rtc) {}

        void setup() {
            logln("Initializing rtc...");
            initializeRtc();

            logln("Initializing lcd...");
            initializeLcd();
            printProgramHeader(&lcd);

            logln("Initializing system...");
            initializeSystem();
        }

        void update() {
            const RtcDateTime time = getTime();
            const JoystickActions jaction = joystick.collectActions();

            actionManager.update(time, jaction);
            system.update(time);
            navigationManager.update(jaction);
        }
};
