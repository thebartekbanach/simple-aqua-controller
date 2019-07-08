#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../menu/TimeField.hpp"

#include "TimeSetupModuleSettings.hpp"

class TimeSetupModule: public CommonSystemModuleWithSettings<TimeSetupModuleSettings> {
    private:
        SystemTime actualTime = { .hour = 0, .minute = 0 };
        bool isInEditMode = false;

        void timeChangedEvent(eventMask event, navNode& nav, prompt &item) {
            if (event == enterEvent && !isInEditMode) {
                isInEditMode = true;
            }
            
            else if (event == exitEvent && isInEditMode) {
                logln("Updating time") logln(actualTime.hour) logln(actualTime.minute);
                rtc->SetDateTime(RtcDateTime(2000, 1, 1, actualTime.hour, actualTime.minute, 0));
                isInEditMode = false;
            }
        }

        void dayCycleSettingsUpdated() {
            settings.saveSettings();
        }

    public:
        TimeSetupModule():
            CommonSystemModuleWithSettings(TimeSetupModuleSettings()) {}

        menuNode** getSettingsMenuItems() {
            prompt** settingsNodes = new prompt*[3] {
                timeField("Aktualna godzina", actualTime,
                    new ExactActionReceiver<TimeSetupModule>(this, &TimeSetupModule::timeChangedEvent), (eventMask)(enterEvent|exitEvent)),
                timeField("Poczatek dnia", settings.data().dayStart,
                    new ActionReceiver<TimeSetupModule>(this, &TimeSetupModule::dayCycleSettingsUpdated)),
                timeField("Koniec dnia", settings.data().nightStart,
                    new ActionReceiver<TimeSetupModule>(this, &TimeSetupModule::dayCycleSettingsUpdated))
            };

            return new menuNode*[1] {
                new menuNode("Czas i cykl dnia", 3, settingsNodes)
            };
        }

        ushort getSettingsMenuItemsLength() { return 1; }

        void update(const RtcDateTime &time) { 
            if (!isInEditMode) {
                this->actualTime = asSystemTime(time);
            }
        }
};
