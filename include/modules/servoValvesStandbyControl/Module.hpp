#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../menu/YesNoToggle.hpp"

#include "../../control/valves/ValveModuleUsingInternalPwm.hpp"

#include "Settings.hpp"

class ServoValvesStandbyControlModule: public CommonSystemModuleWithSettings<ServoValvesStandbyControlModuleSettings> {
    private:
        ValveModuleUsingInternalPwm* valveModule;
        unsigned long* attachTimes;

    public:
        ServoValvesStandbyControlModule(ValveModuleUsingInternalPwm* valveModule):
            valveModule(valveModule),
            CommonSystemModuleWithSettings(ServoValvesStandbyControlModuleSettings()) {
                attachTimes = new unsigned long[valveModule->noOfValves] { 0 };
            }
        
        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto saveSettings = new ActionReceiver<SystemModuleSettings<ServoValvesStandbyControlModuleSettings>>(
                &settings, &SystemModuleSettings<ServoValvesStandbyControlModuleSettings>::saveSettings
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

            for (unsigned short i = 0; i < valveModule->noOfValves; ++i) {
                Servo& servo = valveModule->servoValves[i];
                unsigned long& attachTime = attachTimes[i];

                if (servo.attached() && attachTime == 0) {
                    attachTime = actualTime;
                    continue;
                }

                const unsigned long detachTime = attachTime + settings.data().standbyTimeout;

                if (servo.attached() && attachTime != 0 && actualTime > detachTime) {
                    servo.detach();
                    attachTime = 0;
                }
            }
        }
};