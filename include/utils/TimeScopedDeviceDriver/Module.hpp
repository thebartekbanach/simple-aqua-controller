#pragma once

#include "../../control/relayModule/RelayModule.hpp"

#include "../../menu/TimeField.hpp"
#include "../../menu/TurnStateToggle.hpp"

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/SystemTime.hpp"

#include "../../modules/timeSetup/DayCycle.hpp"
#include "../../modules/timeSetup/Events.hpp"
#include "../../modules/serviceMode/Events.hpp"

#include "../DeviceWorkingMode.hpp"
#include "../isInTimeScope.hpp"

#include "Settings.hpp"

class TimeScopedDeviceDriver: public CommonSystemModuleWithSettings<TimeScopedDeviceDriverSettings> {
    protected:
        char* name;
        const int moduleId;

        RelayModule* relayModule;
        const ushort deviceId;

        DayCycle actualCycle = UNSET;

        TimeScopedDeviceDriver(char* name, const int moduleId, RelayModule* relayModule, const ushort deviceId, const TimeScopedDeviceDriverSettings defaultSettings):
            CommonSystemModuleWithSettings<TimeScopedDeviceDriverSettings>(defaultSettings),
            name(name), moduleId(moduleId), relayModule(relayModule), deviceId(deviceId) {}

    public:
        ushort getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            ActionReceiver<SystemModuleSettings<TimeScopedDeviceDriverSettings>>* settingsUpdated =
                new ActionReceiver<SystemModuleSettings<TimeScopedDeviceDriverSettings>>(
                    &settings, &SystemModuleSettings<TimeScopedDeviceDriverSettings>::saveSettings
                );
                
            prompt** manualWorkTimesMenuItems = new prompt*[2] {
                timeField("Czas wlaczenia", settings.data().start, settingsUpdated),
                timeField("Czas wylaczenia", settings.data().end, settingsUpdated)
            };

            menuNode* manualWorkTimesMenu = new menuNode("Ustawienia zakresu", 2, manualWorkTimesMenuItems);

            prompt** settingsMenuItems = new prompt*[2] {
                turnStateToggle("Tryb: ", settings.data().mode, settingsUpdated),
                manualWorkTimesMenu
            };
            
            menuNode* settingsMenu = new menuNode(name, 2, settingsMenuItems);

            menuNode** result = new menuNode*[1] {
                settingsMenu
            };
            
            return result;
        }

        virtual bool isDeviceEnabled() { return true; }

        virtual void update(const RtcDateTime &time) {
            if (serviceModeActive) return;
            if (!isDeviceEnabled()) return;

            TimeScopedDeviceDriverSettings& settings = this->settings.data();
            
            switch(settings.mode) {
                case FROM_SETTINGS:
                    relayModule->set(deviceId, isInTimeScope(settings.start, settings.end, time));
                    break;

                case CONTINUOUS_ON:
                    relayModule->set(deviceId, ON);
                    break;

                case CONTINUOUS_OFF:
                    relayModule->set(deviceId, OFF);
                    break;

                case IN_DAY_CYCLE:
                case IN_NIGHT_CYCLE:
                    relayModule->set(deviceId,
                        actualCycle == DAY && settings.mode == IN_DAY_CYCLE ? ON :
                        actualCycle == NIGHT && settings.mode == IN_NIGHT_CYCLE ? ON :
                        /* UNSET */ OFF
                    );

                default: break;
            }
        }

        virtual void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) {
            if (moduleId == TIME_SETUP_MODULE_ID) {
                if (eventCode == DAY_CYCLE_BEGIN) {
                    actualCycle = DAY;
                }
                
                else if (eventCode == NIGHT_CYCLE_BEGIN) {
                    actualCycle = NIGHT;
                }
            }

            if (moduleId == SERVICE_MODE_MODULE_ID) {
                serviceModeActive = eventCode;
            }
        };
};
