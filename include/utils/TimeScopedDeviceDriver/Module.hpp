#pragma once

#include "../../control/relayModule/RelayModule.hpp"

#include "../../menu/TimeField.hpp"
#include "../../menu/TurnStateToggle.hpp"

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/SystemTime.hpp"

#include "../../modules/timeSetup/DayCycle.hpp"
#include "../../modules/timeSetup/Events.hpp"

#include "../DeviceWorkingMode.hpp"
#include "../isInTimeScope.hpp"
#include "../log.hpp"

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

            prompt** settingsMenuItems = new prompt*[3] {
                turnStateToggle("Tryb: ", settings.data().mode, settingsUpdated),
                timeField("Czas wlaczenia", settings.data().start, settingsUpdated),
                timeField("Czas wylaczenia", settings.data().end, settingsUpdated)
            };
            
            menuNode* settingsMenu = new menuNode(name, 3, settingsMenuItems);

            menuNode** result = new menuNode*[1] {
                settingsMenu
            };
            
            return result;
        }

        void update(const RtcDateTime &time) {            
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

        void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) {
            if (moduleId == TIME_SETUP_MODULE) {
                if (eventCode == DAY_CYCLE_BEGIN) {
                    actualCycle = DAY;
                }
                
                else if (eventCode == NIGHT_CYCLE_BEGIN) {
                    actualCycle = NIGHT;
                }
            }
        };
};
