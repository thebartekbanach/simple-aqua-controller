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
        virtual uint getSettingsSize() { return 0; }
        virtual void resetSettings() { }

        virtual prompt** getActionMenuItems() { return nullptr; }
        virtual menuNode** getSettingsMenuItems() { return nullptr; }

        virtual unsigned short getActionMenuItemsLength() { return 0; }
        virtual unsigned short getSettingsMenuItemsLength() { return 0; }

        virtual void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage, 
                           GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) { }

        virtual void update(const RtcDateTime &time) { }
        virtual void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) { };
};

class CommonSystemModule: public SystemModule {
    protected:
        GlobalEventBus* eventBus;
        ActionManager* actionManager;
        RtcDS1302<ThreeWire>* rtc;
        
        bool serviceModeActive = false;

        virtual void setup() {}

    public:
        void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage,
                    GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) {
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
        
        bool serviceModeActive = false;

        virtual void setup() {}

    public:
        CommonSystemModuleWithSettings(TSettings defaultSettings): settings(defaultSettings) {}
        
        virtual uint getSettingsSize() { return sizeof(TSettings); }
        virtual void resetSettings() { settings.resetSettings(); }

        void setup(const int &settingsStartAddress, const bool& isEepromInitialized, DueFlashStorage* storage, 
                    GlobalEventBus& eventBus, ActionManager& actionManager, RtcDS1302<ThreeWire>& rtc) {
            this->eventBus = &eventBus;
            this->actionManager = &actionManager;
            this->rtc = &rtc;
            
            settings.initialize(settingsStartAddress, isEepromInitialized, storage);

            this->setup();
        }
};
