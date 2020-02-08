#pragma once

#include "../../utils/DeviceWorkingMode.hpp"
#include "../../system/SystemTime.hpp"

struct SoftLightingSettings {
	SoftLightingSettings(const unsigned short &defaultPowerLevel = 255, const unsigned short &defaultPwmFrequency = 60):
		maximumLightingPower(defaultPowerLevel),
		pwmFrequency(defaultPwmFrequency) {}

    DeviceWorkingMode mode = DeviceWorkingMode::IN_DAY_CYCLE;
    SystemTime start = { .hour = 8, .minute = 0 };
    SystemTime end = { .hour = 20, .minute = 0 };

    SystemTime durationOfMorning = { .hour = 4, .minute = 0 };
    SystemTime durationOfDusk = { .hour = 4, .minute = 0 };
	
	unsigned short pwmFrequency = 60;
	unsigned short maximumLightingPower = 255;
	unsigned short minimumLightingPower = 0;
	unsigned short softLightChangeStepsPerSecond = 255;
};
