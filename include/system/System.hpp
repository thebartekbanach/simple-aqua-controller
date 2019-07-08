#pragma once

#include <menu.h>
#include <RtcDS1302.h>
#include <DueFlashStorage.h>

#include "../program/ActionManager.hpp"

#include "GlobalEventBus.hpp"
#include "SystemModule.hpp"
#include "SystemModulesList.hpp"
#include "MenuItemsResult.hpp"

class System {
    SystemModulesList* modules;
    GlobalEventBus eventBus;

    ActionManager* actionManager;
    RtcDS1302<ThreeWire>* rtc;

    public:
        System(SystemModulesList* modules, ActionManager &actionManager, RtcDS1302<ThreeWire> &rtc):
            modules(modules), actionManager(&actionManager), rtc(&rtc), eventBus(this->modules) {}

        void setup(DueFlashStorage* storage) {
            bool isEepromInitialized = !storage->read(4);
            storage->write(4, false);

            int settingsAddress = 8; // https://github.com/sebnil/DueFlashStorage/blob/master/src/DueFlashStorage.cpp#L63

            for (ushort i = 0; i < modules->length; ++i) {
                modules->items[i]->setup(settingsAddress, isEepromInitialized, storage, eventBus, *actionManager, *rtc);
                settingsAddress += modules->items[i]->getSettingsSize();
            }
        }

        MenuItemsResult<prompt> collectActions() {
            ushort lengthOfActions = 0;

            for (ushort i = 0; i < modules->length; ++i) {
                lengthOfActions += modules->items[i]->getActionMenuItemsLength();
            }
            
            prompt** items = new prompt*[lengthOfActions];

            for (ushort i = 0, total = 0; i < modules->length; ++i) {
                prompt** actions = modules->items[i]->getActionMenuItems();
                ushort actionsLength = modules->items[i]->getActionMenuItemsLength();

                for (ushort j = 0; j < actionsLength; ++j, ++total) {
                    items[total] = actions[j];
                }
            }

            return MenuItemsResult<prompt>(items, lengthOfActions);
        }

        MenuItemsResult<menuNode> collectSettings() {
            ushort lengthOfSettings = 0;

            for (ushort i = 0; i < modules->length; ++i) {
                lengthOfSettings += modules->items[i]->getSettingsMenuItemsLength();
            }
            
            menuNode** items = new menuNode*[lengthOfSettings];

            for (ushort i = 0, total = 0; i < modules->length; ++i) {
                menuNode** settings = modules->items[i]->getSettingsMenuItems();
                ushort settingsLength = modules->items[i]->getSettingsMenuItemsLength();

                for (ushort j = 0; j < settingsLength; ++j, ++total) {
                    items[total] = settings[j];
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