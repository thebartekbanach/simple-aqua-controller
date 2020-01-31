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

		SystemTime lastCorrectDurationOfMorning;
		SystemTime lastCorrectDurationOfDusk;
		
		ActionReceiver<SoftLightingControlModule> *saveSettings =
			new ActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::settingsChanged);

		ExactActionReceiver<SoftLightingControlModule> *validateAndSaveModeAndMiddaySettings =
			new ExactActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::checkAndSaveMiddaySettings);

		SystemTime getDayStart() {
			switch (settings.data().mode) {
				case IN_DAY_CYCLE:
					return dayCycleInfo->getDayStart();

				case IN_NIGHT_CYCLE:
					return dayCycleInfo->getNightStart();

				default:
				case FROM_SETTINGS:
					return settings.data().start;
			}
		}

		SystemTime getDayEnd() {
			switch (settings.data().mode) {
				case IN_DAY_CYCLE:
					return dayCycleInfo->getNightStart();

				case IN_NIGHT_CYCLE:
					return dayCycleInfo->getDayStart();

				default:
				case FROM_SETTINGS:
					return settings.data().end;
			}
		}

		void settingsChanged() {
			settings.saveSettings();
		}

		void checkAndSaveMiddaySettings(eventMask event, navNode& nav, prompt &item) {
			if (event == eventMask::exitEvent) {
				settings.saveSettings();
			}

			else if (
				event == enterEvent
				&& (lastCorrectDurationOfMorning != settings.data().durationOfMorning
				|| lastCorrectDurationOfDusk != settings.data().durationOfDusk)
			) {
				const SystemTime morningEnd = getDayStart() + settings.data().durationOfMorning;
				const SystemTime duskStart = getDayEnd() - settings.data().durationOfDusk;

				if (morningEnd > duskStart) {
					settings.data().durationOfMorning = lastCorrectDurationOfMorning;
					settings.data().durationOfDusk = lastCorrectDurationOfDusk;
				} else { // if durations are ok
					lastCorrectDurationOfMorning = settings.data().durationOfMorning;
					lastCorrectDurationOfDusk = settings.data().durationOfDusk;
				}
			}
		}

		prompt* getLightingTimingSettings() {
			prompt** items = new prompt*[3] {
				turnStateToggle("Tryb: ", settings.data().mode, validateAndSaveModeAndMiddaySettings),
				timeField("Poczatek dnia", settings.data().start, validateAndSaveModeAndMiddaySettings),
				timeField("Koniec dnia", settings.data().end, validateAndSaveModeAndMiddaySettings)
			};
			
			return new menuNode("Tryb pracy", 3, items);
		}

		prompt* getLightingMiddaySettings() {
			prompt** items = new prompt*[2] {
				timeField("Czas switu", settings.data().durationOfMorning, validateAndSaveModeAndMiddaySettings, (eventMask)(exitEvent | enterEvent)),
				timeField("Czas zmierzchu", settings.data().durationOfDusk, validateAndSaveModeAndMiddaySettings, (eventMask)(exitEvent | enterEvent))
			};

			return new menuNode("Poludnie", 2, items);
		}

		prompt* getLightingPowerSettings() {
			prompt** items = new prompt*[3] {
				new menuField<unsigned short>(settings.data().maximumLightingLevel, "Max poziom", "", 1, 255, 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().minimumLightingLevel, "Min poziom", "", 0, 254, 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().softLightChangeStepsPerSecond, "Krok zmiany", "", 1, 255 * 4, 10, 1, saveSettings, exitEvent)
			};

			return new menuNode("Moc i krok zmiany", 3, items);
		}

		unsigned short getNextLightingLevel(const RtcDateTime &time) {
            switch (settings.data().mode) {
				case DeviceWorkingMode::IN_DAY_CYCLE:
				case DeviceWorkingMode::IN_NIGHT_CYCLE:
				case DeviceWorkingMode::FROM_SETTINGS:
					return light.update(
						time.TotalSeconds64(),
						getDayStart(),
						getDayEnd(),
						getDayStart() + settings.data().durationOfMorning,
						getDayEnd() - settings.data().durationOfDusk
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
			lastCorrectDurationOfMorning(settings.data().durationOfMorning),
			lastCorrectDurationOfDusk(settings.data().durationOfDusk),
			light(
				settings.data().softLightChangeStepsPerSecond,
				settings.data().maximumLightingLevel,
				settings.data().minimumLightingLevel
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
				light.reset();
            }
        }
};
