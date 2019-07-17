#pragma once

#include "../../system/SystemTime.hpp"
#include "../../utils/DeviceWorkingMode.hpp"

struct WaterAdditionModuleSettings {
    DeviceWorkingMode mode = IN_DAY_CYCLE;
    SystemTime canWorkFrom = { .hour = 8, .minute = 0 };
    SystemTime canWorkTo = { .hour = 20, .minute = 0 };

    unsigned short breaksBetweenChecks = 60; // minutes
    unsigned short numberOfChecks = 10;
    unsigned short checkingFrequency = 1000; // miliseconds
    unsigned short minNumberOfUnsuccessfullAttemps = 70; // %
};