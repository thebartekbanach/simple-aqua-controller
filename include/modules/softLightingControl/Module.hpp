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
#include "../../control/pwmLightController/PwmLightController.hpp"

#include "LightDriver.hpp"
#include "Settings.hpp"

class SoftLightingControlModule: public CommonSystemModuleWithSettings<SoftLightingSettings> {
    private:
		PwmLightController* driver;
		DayCycleInfo* dayCycleInfo;
		LightDriver light;

		SystemTime lastCorrectDurationOfMorning;
		SystemTime lastCorrectDurationOfDusk;

		unsigned short lastPwmValue = 0;

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
				settingsChanged();
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

		void checkAndSetValidMiddaySettingsIfActualAreInvalid() {
			const SystemTime morningEnd = getDayStart() + settings.data().durationOfMorning;
			const SystemTime duskStart = getDayEnd() - settings.data().durationOfDusk;

			if (morningEnd > duskStart) {
				// set default morning duration and dusk duration to 1/3 of day for each one
				const unsigned int durationOfDay = distanceBetween(getDayStart(), getDayEnd());

				settings.data().durationOfMorning = asSystemTime(durationOfDay / 3);
				settings.data().durationOfDusk = asSystemTime(durationOfDay / 3);
			}
		}

		void saveModeAndValidateMiddaySettings(eventMask event, navNode& nav, prompt &item) {
			if (
				settings.data().mode == DeviceWorkingMode::CONTINUOUS_OFF ||
				settings.data().mode == DeviceWorkingMode::CONTINUOUS_ON
			) {
				settingsChanged();
				return;
			}

			checkAndSetValidMiddaySettingsIfActualAreInvalid();
			settingsChanged();
		}

		void updateOrSaveFrequencySetting(eventMask event, navNode& nav, prompt &item) {
			if (event == eventMask::exitEvent) {
				settingsChanged();
			}
			
			if (driver->isPwmFrequencySetupAvailable()) {
				driver->setPwmFrequency(settings.data().pwmFrequency);
			}
		}

		prompt* getLightingTimingSettings() {
			auto *saveModeAndValidateMiddaySettingsEvent =
				new ExactActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::saveModeAndValidateMiddaySettings);

			prompt** items = new prompt*[3] {
				turnStateToggle("Tryb: ", settings.data().mode, saveModeAndValidateMiddaySettingsEvent, (eventMask)(exitEvent | enterEvent)),
				timeField("Poczatek dnia", settings.data().start, saveModeAndValidateMiddaySettingsEvent, (eventMask)(exitEvent | enterEvent)),
				timeField("Koniec dnia", settings.data().end, saveModeAndValidateMiddaySettingsEvent, (eventMask)(exitEvent | enterEvent))
			};
			
			return new menuNode("Tryb pracy", 3, items);
		}

		prompt* getLightingMiddaySettings() {
			auto *validateAndSaveMiddaySettingsEvent =
				new ExactActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::checkAndSaveMiddaySettings);
			
			prompt** items = new prompt*[2] {
				timeField("Dlugosc switu", settings.data().durationOfMorning, validateAndSaveMiddaySettingsEvent, (eventMask)(exitEvent | enterEvent)),
				timeField("Dlugosc zmierzchu", settings.data().durationOfDusk, validateAndSaveMiddaySettingsEvent, (eventMask)(exitEvent | enterEvent))
			};

			return new menuNode("Poludnie", 2, items);
		}

		prompt* getLightingPowerSettings() {
			auto *saveSettings =
				new ActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::settingsChanged);

			auto *updateOrSaveFrequencySettingEvent =
				new ExactActionReceiver<SoftLightingControlModule>(this, &SoftLightingControlModule::updateOrSaveFrequencySetting);

			prompt** items = new prompt*[4] {
				new menuField<unsigned short>(settings.data().maximumLightingPower, "Max poziom", "", 1, driver->getMaximumPowerLevel(), 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().minimumLightingPower, "Min poziom", "", 0, driver->getMaximumPowerLevel() - 1, 10, 1, saveSettings, exitEvent),
				new menuField<unsigned short>(settings.data().pwmFrequency, "Czestot.", "hz", driver->getMinimumPwmFrequency(), driver->getMaximumPwmFrequency(), 10, 1, updateOrSaveFrequencySettingEvent, anyEvent),
				new menuField<unsigned short>(settings.data().softLightChangeStepsPerSecond, "Krok zmiany", "", 1, driver->getMaximumPwmFrequency(), 10, 1, saveSettings, exitEvent)
			};

			if (!driver->isPwmFrequencySetupAvailable()) {
				items[2]->disable();
			}

			return new menuNode("Moc i krok zmiany", 4, items);
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

		void setup() override {
			if (driver->isPwmFrequencySetupAvailable()) {
				driver->setPwmFrequency(settings.data().pwmFrequency);
			}
		}
        
    public:
		SoftLightingControlModule(PwmLightController *pwmDriver, DayCycleInfo* dayCycleInfo):
			driver(pwmDriver),
			dayCycleInfo(dayCycleInfo),
			CommonSystemModuleWithSettings<SoftLightingSettings>(SoftLightingSettings(
				pwmDriver->getMaximumPowerLevel(),
				pwmDriver->getMaximumPwmFrequency()
			)),
			lastCorrectDurationOfMorning(settings.data().durationOfMorning),
			lastCorrectDurationOfDusk(settings.data().durationOfDusk),
			light(
				settings.data().softLightChangeStepsPerSecond,
				settings.data().maximumLightingPower,
				settings.data().minimumLightingPower
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
				driver->setLightPower(newPwmValue);
			}
        }

		void onEvent(const int &moduleId, const int &eventCode, void* data) {
			switch (moduleId) {
				case SERVICE_MODE_MODULE_ID:
					serviceModeActive = eventCode;

					driver->setLightPower(0);

					if (driver->isPwmFrequencySetupAvailable()) {
						driver->setPwmFrequency(settings.data().pwmFrequency);
					}

					light.reset();

					break;
				
				case TIME_SETUP_MODULE_ID:
					if (eventCode == DAY_CYCLE_CHANGED) {
						checkAndSetValidMiddaySettingsIfActualAreInvalid();
					}
					
					break;
				
				default: break;
			}
		}
};
