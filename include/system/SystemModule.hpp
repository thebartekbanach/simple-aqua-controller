#pragma once
#include <menu.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>

#include "MenuItemsResult.hpp"
#include "../program/ActionManager.hpp"

class GlobalEventBus;

class SystemModule {
    public:
        virtual int getSettingsSize() { return 0; }
        virtual void resetSettings() { }

        virtual MenuItemsResult<prompt> getActionsMenu() { return MenuItemsResult<prompt>(nullptr, 0); }
        virtual MenuItemsResult<menuNode> getSettingsMenu(){ return MenuItemsResult<menuNode>(nullptr, 0); }

        virtual void setup(const int &settingsStartAddress, GlobalEventBus& eventBus, ActionManager& actionManager) { }
        virtual void update(const RtcDateTime &time) { }
        virtual void onEvent(const int &moduleId, const int &eventCode) { };
};

class CommonSystemModule: public SystemModule {
    protected:
        GlobalEventBus* eventBus;
        ActionManager* actionManager;
        int settingsStartAddress;

        virtual void setup() {}

    public:
        void setup(const int &settingsStartAddress, GlobalEventBus& eventBus, ActionManager& actionManager) {
            this->settingsStartAddress = settingsStartAddress;
            this->eventBus = &eventBus;
            this->actionManager = &actionManager;

            this->setup();
        }
};
