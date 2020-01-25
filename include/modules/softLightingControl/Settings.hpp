#pragma once

#include "../../utils/DeviceWorkingMode.hpp"
#include "../../system/SystemTime.hpp"

struct SoftLightingSettings {
    DeviceWorkingMode mode = DeviceWorkingMode::IN_DAY_CYCLE;
    SystemTime start = { .hour = 8, .minute = 0 };
    SystemTime end = { .hour = 20, .minute = 0 };

    SystemTime midday = { .hour = 12, .minute = 0 };
    SystemTime middayLength = { .hour = 2, .minute = 0 };
	
	unsigned short maximumLightingLevel = 4096;
	unsigned short minimumLightingLevel = 0;
	unsigned short softLightChangeStepsPerSecond = 1000;
};
