#pragma once

#include <menu.h>
#include <RtcDS1302.h>

#include "../program/ActionManager.hpp"

#include "GlobalEventBus.hpp"
#include "SystemModule.hpp"
#include "SystemModulesList.hpp"
#include "MenuItemsResult.hpp"

#include "../utils/log.hpp"

class System {
    SystemModulesList* modules;
    GlobalEventBus eventBus;

    ActionManager* actionManager;
    RtcDS1302<ThreeWire>* rtc;

    public:
        System(SystemModulesList* modules, ActionManager &actionManager, RtcDS1302<ThreeWire> &rtc):
            modules(modules), actionManager(&actionManager), rtc(&rtc), eventBus(this->modules) {}

        void setup() {
            int settingsAddress = 0;

            for (ushort i = 0; i < modules->length; ++i) {
                modules->items[i]->setup(settingsAddress, eventBus, *actionManager);
                settingsAddress += modules->items[i]->getSettingsSize();
            }
        }

        MenuItemsResult<prompt> collectActions() {
            ushort lengthOfActions = 0;

            for (ushort i = 0; i < modules->length; ++i) {
                lengthOfActions += modules->items[i]->getActionsMenu().length;
            }

            log("Length of actions is: ") logln(lengthOfActions);
            
            prompt** items = new prompt*[lengthOfActions];

            for (ushort i = 0, total = 0; i < lengthOfActions; ++i) {
                auto actions = modules->items[i]->getActionsMenu();

                for (ushort j = 0; j < actions.length; ++j, ++total) {
                    items[total] = actions.items[j];
                }
            }

            return MenuItemsResult<prompt>(items, lengthOfActions);
        }

        MenuItemsResult<menuNode> collectSettings() {
            ushort lengthOfSettings = 0;

            for (ushort i = 0; i < modules->length; ++i) {
                lengthOfSettings += modules->items[i]->getActionsMenu().length;
            }

            menuNode** items = new menuNode*[lengthOfSettings];

            for (ushort i = 0, total = 0; i < lengthOfSettings; ++i) {
                auto settings = modules->items[i]->getSettingsMenu();

                for (ushort j = 0; j < settings.length; ++j, ++total) {
                    items[total] = settings.items[j];
                }
            }

            return MenuItemsResult<menuNode>(items, lengthOfSettings);
        }

        void update(const RtcDateTime &time) {
            for (ushort i = 0; i < modules->length; ++i) {
                modules->items[i]->update(time);
            }
        }
};