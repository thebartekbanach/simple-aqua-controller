#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../menu/YesNoToggle.hpp"

#include "../../control/valves/ValveModule.hpp"

#include "Settings.hpp"

class ServoValvesStandbyControlModule: public CommonSystemModuleWithSettings<ServoValvesStandbyControlModuleSettings> {
    private:
        ValveModuleAttachManagement* valveModule;
        unsigned long* attachTimes;

        void saveSettings() {
            settings.saveSettings();
        }

    public:
        ServoValvesStandbyControlModule(ValveModuleAttachManagement* valveModule):
            valveModule(valveModule),
            CommonSystemModuleWithSettings(ServoValvesStandbyControlModuleSettings()) {
                attachTimes = new unsigned long[valveModule->getNumberOfValves()] { 0 };
            }
        
        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto saveSettings = new ActionReceiver<ServoValvesStandbyControlModule>(
                this, &ServoValvesStandbyControlModule::saveSettings
            );

            prompt* autoDetachEnabled = YesNoToggle(
                "Auto odlaczan.: ",
                settings.data().standbyControlActive,
                saveSettings
            );

            prompt* autoDetachTimeout = new menuField<unsigned short>(settings.data().standbyTimeout,
                "A. odl. tout:", "s", 5, 60, 1, 0, saveSettings
            );

            prompt** settingItems = new prompt*[2] {
                autoDetachEnabled,
                autoDetachTimeout
            };

            return new menuNode*[1] {
                new menuNode("Serwozawory", 2, settingItems)
            };
        }

        void update(const RtcDateTime &time) {
            if (!settings.data().standbyControlActive) return;
            
            const unsigned long actualTime = time.TotalSeconds64();

            for (unsigned short i = 0; i < valveModule->getNumberOfValves(); ++i) {
                if (valveModule->attached(i) && attachTimes[i] == 0) {
                    attachTimes[i] = actualTime;
                    continue;
                }

                if (attachTimes[i] == 0) continue;
                
                const unsigned long detachTime = attachTimes[i] + settings.data().standbyTimeout;
                
                if (actualTime <= detachTime) continue;
                
                attachTimes[i] = 0;

                if (valveModule->attached(i) ) {
                    log("Detaching servovalve: ") logln(i);
                    valveModule->detach(i);
                }
            }
        }
};