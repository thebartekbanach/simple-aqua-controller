#pragma once

#include "../../program/TimeGuard.hpp"

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../menu/TimeField.hpp"

#include "../../utils/isInTimeScope.hpp"

#include "../serviceMode/Events.hpp"

#include "DayCycleInfo.hpp"

#include "Events.hpp"
#include "DayCycle.hpp"
#include "Settings.hpp"

class TimeSetupModule:
	public CommonSystemModuleWithSettings<TimeSetupModuleSettings>,
	public DayCycleInfo {
		
    private:
        TimeGuard* timeGuard;

        SystemTime actualTime = { .hour = 0, .minute = 0 };
        bool isInEditMode = false;

        DayCycle actualCycle = UNSET;

        void timeChangedEvent(eventMask event, navNode& nav, prompt &item) {
            if (event == enterEvent && !isInEditMode) {
                isInEditMode = true;
            }
            
            else if (event == exitEvent && isInEditMode) {
                timeGuard->setTime(RtcDateTime(2000, 1, 1, actualTime.hour, actualTime.minute, 0));
                isInEditMode = false;
            }
        }

        void dayCycleSettingsUpdated() {
			eventBus->send(TIME_SETUP_MODULE_ID, DAY_CYCLE_CHANGED);
            settings.saveSettings();
        }

    public:
        TimeSetupModule(TimeGuard* timeGuard):
            timeGuard(timeGuard),
            CommonSystemModuleWithSettings(TimeSetupModuleSettings()) {}

		const SystemTime& getDayStart() { return settings.data().dayStart; }
		const SystemTime& getNightStart() { return settings.data().nightStart; }

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

        unsigned short getSettingsMenuItemsLength() { return 1; }

        void update(const RtcDateTime &time) { 
            if (isInEditMode || serviceModeActive) return;

            actualTime = asSystemTime(time);

            bool isDay = isInTimeScope(settings.data().dayStart, settings.data().nightStart, actualTime);

            if (isDay && actualCycle != DAY) {
                actualCycle = DAY;
                eventBus->send(TIME_SETUP_MODULE_ID, DAY_CYCLE_BEGIN);
                return;
            }

            if (!isDay && actualCycle != NIGHT) {
                actualCycle = NIGHT;
                eventBus->send(TIME_SETUP_MODULE_ID, NIGHT_CYCLE_BEGIN);
                return;
            }
        }

        void onEvent(const int &moduleId, const int &eventCode, void* data) {
            if (moduleId == SERVICE_MODE_MODULE_ID) {
                serviceModeActive = eventCode;
            }
        }
};
