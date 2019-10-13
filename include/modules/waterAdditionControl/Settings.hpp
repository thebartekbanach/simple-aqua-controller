#pragma once

#include "../../system/SystemTime.hpp"

enum WAM_WorkingMode {
    DISABLED = 0,
    CONTINUOUS = 1,
    MANUAL = 2,
    DAY_CYCLE = 3,
    NIGHT_CYCLE = 4
};

struct WaterAdditionModuleSettings {
    WAM_WorkingMode workingMode = DAY_CYCLE;
    SystemTime manualWorkStart = { .hour = 8, .minute = 0 };
    SystemTime manualWorkEnd = { .hour = 20, .minute = 0 };
    unsigned short workTimeShift = 5; // minutes

    unsigned short breaksBetweenChecks = 60; // minutes
    unsigned short numberOfChecks = 10;
    unsigned short checkingFrequency = 200; // seconds
    unsigned short minNumberOfUnsuccessfullAttempts = 70; // %

    float waterAdditionTimeout = 5; // minutes
    float addionalWaterTankRefillTimeout = 5; // minutes
};
