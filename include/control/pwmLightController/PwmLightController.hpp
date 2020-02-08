#pragma once

class PwmLightController {
	public:
		virtual bool isPwmFrequencySetupAvailable() { return false; }
		virtual void setPwmFrequency(const unsigned short &frequency) { }

		virtual unsigned short getMaximumPwmFrequency() { return 1024; }
		virtual unsigned short getMinimumPwmFrequency() { return 10; }

		virtual unsigned short getMaximumPowerLevel() { return 255; }

		virtual void setLightPower(const unsigned short &value) = 0;
};