#pragma once
#include <menu.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <DueFlashStorage.h>

#include "MenuItemsResult.hpp"
#include "SystemModuleSettings.hpp"
#include "../program/ActionManager.hpp"

class GlobalEventBus;

class SystemModule {
    public:
        virtual int getSettingsSize() { return 0; }
        virtual void resetSettings() { }

        virtual MenuItemsResult<prompt> getActionsMenu() { return MenuItemsResult<prompt>(nullptr, 0); }
        virtual MenuItemsResult<menuNode> getSettingsMenu(){ return MenuItemsResult<menuNode>(nullptr, 0); }

        virtual void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage, 
                           GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) { }
        virtual void update(const RtcDateTime &time) { }
        virtual void onEvent(const int &moduleId, const int &eventCode) { };
};

class CommonSystemModule: public SystemModule {
    protected:
        GlobalEventBus* eventBus;
        ActionManager* actionManager;
        RtcDS1302<ThreeWire>* rtc;
        int settingsStartAddress;

        virtual void setup() {}

    public:
        void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage,
                    GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) {
            this->settingsStartAddress = settingsStartAddress;
            this->eventBus = &eventBus;
            this->actionManager = &actionManager;
            this->rtc = &rtc;

            this->setup();
        }
};

template<typename TSettings>
class CommonSystemModuleWithSettings: public SystemModule {
    protected:
        GlobalEventBus* eventBus;
        ActionManager* actionManager;
        RtcDS1302<ThreeWire>* rtc;
        SystemModuleSettings<TSettings> settings;

        virtual void setup() {}

    public:
        CommonSystemModuleWithSettings(TSettings defaultSettings): settings(defaultSettings) {}

        void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage, 
                    GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) {
            this->eventBus = &eventBus;
            this->actionManager = &actionManager;
            this->rtc = &rtc;
            
            settings.initialize(settingsStartAddress, isEepromInitialized, storage);

            this->setup();
        }
};
