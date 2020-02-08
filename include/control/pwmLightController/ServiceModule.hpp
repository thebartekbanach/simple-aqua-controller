#pragma once

#include "PwmLightController.hpp"

#include "../../modules/serviceMode/ServiceModule.hpp"

class PwmLightControllerServiceModule: public ServiceModule {
	private:
		PwmLightController* controller = nullptr;

		unsigned short pwmFrequency = 0;
		unsigned short power = 0;

		void valuesUpdated() {
			if (controller->isPwmFrequencySetupAvailable()) {
				controller->setPwmFrequency(pwmFrequency);
			}

			controller->setLightPower(power);
		}

		void reset() {
			pwmFrequency = controller->getMinimumPwmFrequency();
			power = 0;
			
			valuesUpdated();
		}

	public:
		PwmLightControllerServiceModule(PwmLightController* controller):
			controller(controller) {
				pwmFrequency = controller->getMinimumPwmFrequency();
			}
			
		void activateModule() {
			reset();
		}

		void deactivateModule() {
			reset();
		}

		prompt* getServiceMenu() {
			auto updateAction = 
				new ActionReceiver<PwmLightControllerServiceModule>(this, &PwmLightControllerServiceModule::valuesUpdated);

			prompt** items = new prompt*[2] {
				new menuField<unsigned short>(
					pwmFrequency,
					"Czestot.", "hz",
					controller->getMinimumPwmFrequency(),
					controller->getMaximumPwmFrequency(),
					10, 1,
					updateAction,
					enterEvent
				),
				new menuField<unsigned short>(
					power,
					"Moc", "",
					0, controller->getMaximumPowerLevel(),
					10, 1,
					updateAction,
					enterEvent
				)
			};

			if (!controller->isPwmFrequencySetupAvailable()) {
				items[0]->disable();
			}

			return new menuNode("Oswietlenie", 2, items);
		};
};