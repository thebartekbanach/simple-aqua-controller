#pragma once

#include <Arduino.h>

#include "../../modules/softLightingControl/PwmLightController.hpp"

class PwmLightControllerUsingInternalPwm: public PwmLightController {
	private:
		const unsigned short pwmLightControlPin;

		unsigned short lastPwmValue = 0;

	public:
		PwmLightControllerUsingInternalPwm(const unsigned short &pwmLightControlPin = 6, const unsigned short &pwmFrequency = 70):
			pwmLightControlPin(pwmLightControlPin) {
				analogWrite(pwmLightControlPin, 0); // setup PWM interface
				PWMC_ConfigureClocks(pwmFrequency * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK); // set PWM frequency
		}

		void setLightPwm(const unsigned short &value) {
			if (value != lastPwmValue) {
				lastPwmValue = value;
				analogWrite(pwmLightControlPin, value);
			}
		}
};
