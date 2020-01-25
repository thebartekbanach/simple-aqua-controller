#pragma once

#include <Adafruit_PWMServoDriver.h>

#include "../../modules/softLightingControl/PwmLightController.hpp"

class PwmLightControllerUsingAdafruitPwmServoDriver: public PwmLightController {
	private:
		Adafruit_PWMServoDriver* driver;
		unsigned short lightSteeringPin;

	public:
		PwmLightControllerUsingAdafruitPwmServoDriver(
			Adafruit_PWMServoDriver* driver,
			unsigned short lightSteeringPin
		):
			driver(driver),
			lightSteeringPin(lightSteeringPin) {}

		PwmLightControllerUsingAdafruitPwmServoDriver(
            unsigned short i2cAddress,
            TwoWire& driverConnection,
            float pwmFrequency,
			unsigned short lightSteeringPin
		):
			lightSteeringPin(lightSteeringPin)
		{
			driver = new Adafruit_PWMServoDriver(i2cAddress, driverConnection);

			driver->begin();
			driver->setPWMFreq(pwmFrequency);
		}

		void setLightPwm(const unsigned short &value) {
			driver->setPWM(lightSteeringPin, 0, value);
		}
};