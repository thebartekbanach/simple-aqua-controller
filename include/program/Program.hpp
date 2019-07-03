#pragma once

#include <Arduino.h>

#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/liquidCrystalOut.h>
#include <LiquidCrystal_PCF8574.h>

#include <ThreeWire.h>
#include <RtcDS1302.h>

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

    navNode nav_cursors[MAX_DEPTH];
    navRoot nav;

    System system;
    ActionManager actionManager;
    NavigationManager navigationManager;

    void initializeRtc() {
        RtcDateTime compiled = RtcDateTime(__DATE__, "00:00:00");

        if (!rtc.IsDateTimeValid()) 
        {
            // Common Causes:
            //    1) first time you ran and the device wasn't running yet
            //    2) the battery on the device is low or even missing
            rtc.SetDateTime(compiled);
        }

        if (rtc.GetIsWriteProtected())
        {
            rtc.SetIsWriteProtected(false);
        }

        if (!rtc.GetIsRunning())
        {
            rtc.SetIsRunning(true);
        }

        RtcDateTime now = rtc.GetDateTime();

        if (now < compiled) 
        {
            rtc.SetDateTime(compiled);
        }
    }

    void buildMenu(Menu::prompt** actionItems, ushort actionsLength, Menu::menuNode** settingItems, ushort settingsSize) {
        menuNode* settingsMenu = new menuNode("Ustawienia", settingsSize, (prompt**)settingItems, noAction, noEvent, wrapStyle);

        prompt** mainMenuItems = combineMenuParts(actionItems, actionsLength, (prompt*)settingsMenu);
        menuNode* mainMenu = new menuNode("Menu glowne", actionsLength + 1, mainMenuItems, noAction, noEvent, wrapStyle);

        log("Final length of actions is: ") logln(actionsLength + 1);
        logln(mainMenuItems[0]->getText());
        logln(mainMenuItems[1]->getText());
        nav.useMenu(*mainMenu);
    }

    void initializeSystem() {
        logln("Starting system.setup()")
        system.setup();

        logln("Collecting actions and settings")
        MenuItemsResult<Menu::prompt> actions = system.collectActions();
        MenuItemsResult<Menu::menuNode> settings = system.collectSettings();

        logln("Starting menu build...")
        buildMenu(actions.items, actions.length, settings.items, settings.length);
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
            const RtcDateTime time = rtc.GetDateTime();
            const JoystickActions jaction = joystick.collectActions();

            system.update(time);
            actionManager.update(time, jaction);
            navigationManager.update(jaction);
        }
};
