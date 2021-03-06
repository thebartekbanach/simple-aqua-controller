#pragma once

#include <menu.h>
#include <RtcDS1302.h>
#include <DueFlashStorage.h>

#include "../program/ActionManager.hpp"

#include "GlobalEventBus.hpp"
#include "SystemModule.hpp"
#include "SystemModulesList.hpp"
#include "MenuItemsResult.hpp"
#include "ActionReceiver.hpp"

#include "DefaultSettingsRestoreConfirmActionCreator.hpp"

class System {
    SystemModulesList* modules;
    GlobalEventBus eventBus;

    ActionManager* actionManager;
    RtcDS1302<ThreeWire>* rtc;

    void restoreDefaultSettings() {
        for (unsigned short i = 0; i < modules->length; ++i) {
            modules->items[i]->resetSettings();
        }
    }

    void startDefaultSettingsRestoreCreator() {
        if (actionManager->isUsedByCreator()) return;

        actionManager->acquire(new DefaultSettingsRestoreConfirmActionCreator<System>(this, &System::restoreDefaultSettings));
    }

    public:
        System(SystemModulesList* modules, ActionManager &actionManager, RtcDS1302<ThreeWire> &rtc):
            modules(modules), actionManager(&actionManager), rtc(&rtc), eventBus(this->modules) {}

        void setup(DueFlashStorage* storage) {
            bool isEepromInitialized = !storage->read(4);
            storage->write(4, false);

            int settingsAddress = 8; // https://github.com/sebnil/DueFlashStorage/blob/master/src/DueFlashStorage.cpp#L63

            for (unsigned short i = 0; i < modules->length; ++i) {
                modules->items[i]->setup(settingsAddress, isEepromInitialized, storage, eventBus, *actionManager, *rtc);
                settingsAddress += modules->items[i]->getSettingsSize();

                // Flash start address must be on four byte boundary error fix
                settingsAddress += settingsAddress % 4;
            }
        }

        MenuItemsResult<prompt> collectActions() {
            unsigned short lengthOfActions = 0;

            for (unsigned short i = 0; i < modules->length; ++i) {
                lengthOfActions += modules->items[i]->getActionMenuItemsLength();
            }
            
            prompt** items = new prompt*[lengthOfActions];

            for (unsigned short i = 0, total = 0; i < modules->length; ++i) {
                prompt** actions = modules->items[i]->getActionMenuItems();
                unsigned short actionsLength = modules->items[i]->getActionMenuItemsLength();

                for (unsigned short j = 0; j < actionsLength; ++j, ++total) {
                    items[total] = actions[j];
                }
            }

            return MenuItemsResult<prompt>(items, lengthOfActions);
        }

        MenuItemsResult<prompt> collectSettings() {
            unsigned short lengthOfSettings = 0;

            for (unsigned short i = 0; i < modules->length; ++i) {
                lengthOfSettings += modules->items[i]->getSettingsMenuItemsLength();
            }
            
            prompt** items = new prompt*[lengthOfSettings + 1];

            for (unsigned short i = 0, total = 0; i < modules->length; ++i) {
                menuNode** settings = modules->items[i]->getSettingsMenuItems();
                unsigned short settingsLength = modules->items[i]->getSettingsMenuItemsLength();

                for (unsigned short j = 0; j < settingsLength; ++j, ++total) {
                    items[total] = settings[j];
                }
            }

            items[lengthOfSettings] = new prompt("Przywroc fabryczne",
                new ActionReceiver<System>(this, &System::startDefaultSettingsRestoreCreator),
                enterEvent);

            return MenuItemsResult<prompt>(items, lengthOfSettings + 1);
        }

        void update(const RtcDateTime &time) {
            for (unsigned short i = 0; i < modules->length; ++i) {
                modules->items[i]->update(time);
            }
        }
};