#pragma once
#include <Adafruit_PWMServoDriver.h>

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../menu/TimeField.hpp"
#include "../../menu/TurnStateToggle.hpp"

#include "../timeSetup/DayCycleInfo.hpp"
#include "../timeSetup/Events.hpp"
#include "../serviceMode/Events.hpp"

#include "../../control/pwmLightController/PwmLightControllerUsingAdafruitPwmServoDriver.hpp"

#include "PwmLightController.hpp"
#include "LightDriver.hpp"
#include "Settings.hpp"

class SoftLightingControlModule: public CommonSystemModuleWithSettings<SoftLightingSettings> {
    private:
		PwmLightController* driver;
		DayCycleInfo* dayCycleInfo;
		LightDriver light;

		unsigned short lastPwmValue = 0;

		void settingsChanged() {
			settings.saveSettings();
		}
		
		ActionReceiver<SoftLightingControlModule> *saveSettings =
			new ActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::settingsChanged);

		prompt* getLightingTimingSettings() {
			prompt** items = new prompt*[3] {
				turnStateToggle("Tryb: ", settings.data().mode, saveSettings),
				timeField("Poczatek dnia", settings.data().start, saveSettings),
				timeField("Koniec dnia", settings.data().end, saveSettings)
			};
			
			return new menuNode("Tryb pracy", 3, items);
		}

		prompt* getLightingMiddaySettings() {
			prompt** items = new prompt*[2] {
				timeField("Poczatek poludnia", settings.data().midday, saveSettings),
				timeField("Czas trw. poludnia", settings.data().middayLength, saveSettings)
			};

			return new menuNode("Poludnie", 2, items);
		}

		prompt* getLightingPowerSettings() {
			prompt** items = new prompt*[3] {
				new menuField<unsigned short>(settings.data().maximumLightingLevel, "Max poziom", "", 1, 4096, 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().minimumLightingLevel, "Min poziom", "", 0, 4095, 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().softLightChangeStepsPerSecond, "Krok zmiany", "", 1, 4096 * 4, 10, 1, saveSettings, exitEvent)
			};

			return new menuNode("Moc i szybkosc", 3, items);
		}

		unsigned short getNextLightingLevel(const RtcDateTime &time) {
            switch (settings.data().mode) {
				case DeviceWorkingMode::IN_DAY_CYCLE:
					return light.update(
						asSystemTime(time),
						dayCycleInfo->getDayStart(),
						dayCycleInfo->getNightStart(),
						settings.data().midday,
						settings.data().midday + settings.data().middayLength
					);

				case DeviceWorkingMode::IN_NIGHT_CYCLE:
					return light.update(
						asSystemTime(time),
						dayCycleInfo->getNightStart(),
						dayCycleInfo->getDayStart(),
						settings.data().midday,
						settings.data().midday + settings.data().middayLength
					);

				case DeviceWorkingMode::FROM_SETTINGS:
					return light.update(
						asSystemTime(time),
						settings.data().start,
						settings.data().end,
						settings.data().midday,
						settings.data().midday + settings.data().middayLength
					);
					
				case DeviceWorkingMode::CONTINUOUS_ON:
					return light.turnLights(true);
				
				default:
				case DeviceWorkingMode::CONTINUOUS_OFF:
					return light.turnLights(false);
			}
        }

        
    public:
		SoftLightingControlModule(PwmLightController *pwmDriver, DayCycleInfo* dayCycleInfo):
			driver(pwmDriver),
			dayCycleInfo(dayCycleInfo),
			CommonSystemModuleWithSettings<SoftLightingSettings>(SoftLightingSettings()),
			light(
				&settings.data().softLightChangeStepsPerSecond,
				&settings.data().maximumLightingLevel,
				&settings.data().minimumLightingLevel
			) {}

		SoftLightingControlModule(
			Adafruit_PWMServoDriver* servoDriver,
			unsigned short lightSteeringPin,
			DayCycleInfo* dayCycleInfo):
				SoftLightingControlModule(
					new PwmLightControllerUsingAdafruitPwmServoDriver(servoDriver, lightSteeringPin),
					dayCycleInfo
				) {} 

        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            prompt** submenus = new prompt*[3] {
				getLightingTimingSettings(),
				getLightingMiddaySettings(),
				getLightingPowerSettings()
            };

            menuNode* settingsMenu = new menuNode("Oswietlenie", 3, submenus);

            return new menuNode*[1] {
                settingsMenu
            };
        }

		void update(const RtcDateTime &time) {
			if (serviceModeActive) {
				return;
			}

			const unsigned short newPwmValue = getNextLightingLevel(time);

			if (lastPwmValue != newPwmValue) {
				lastPwmValue = newPwmValue;
				driver->setLightPwm(newPwmValue);
			}
        }

		void onEvent(const int &moduleId, const int &eventCode, void* data) {
            if (moduleId == SERVICE_MODE_MODULE_ID) {
                serviceModeActive = eventCode;
				driver->setLightPwm(0);
            }
        }
};
