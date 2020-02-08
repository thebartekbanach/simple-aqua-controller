#pragma once

#include <Arduino.h>

#include "PwmLightController.hpp"

class PwmLightControllerUsingInternalPwm: public PwmLightController {
	private:
		const unsigned short pwmLightControlPin;
		unsigned short lastPwmValue = 0;

	public:
		PwmLightControllerUsingInternalPwm(const unsigned short &pwmLightControlPin = 6, const unsigned short &defaultPwmFrequency = 60):
			pwmLightControlPin(pwmLightControlPin) {
				analogWrite(pwmLightControlPin, 0); // setup PWM interface
				setPwmFrequency(defaultPwmFrequency);
		}
		
		bool isPwmFrequencySetupAvailable() { return true; }
		
		void setPwmFrequency(const unsigned short &frequency) {
			PWMC_ConfigureClocks(frequency * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK);
		};

		void setLightPower(const unsigned short &value) {
			if (value != lastPwmValue) {
				lastPwmValue = value;
				analogWrite(pwmLightControlPin, value);
			}
		}
};
