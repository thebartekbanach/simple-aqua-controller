#pragma once

#include "../../system/SystemTime.hpp"

enum WAM_WorkingMode {
    DISABLED = 0,
    MANUAL = 1,
    DAY_CYCLE = 2,
    NIGHT_CYCLE = 3
};

struct WaterAdditionModuleSettings {
    WAM_WorkingMode workingMode = DAY_CYCLE;
    SystemTime manualWorkStart = { .hour = 8, .minute = 0 };
    SystemTime manualWorkEnd = { .hour = 20, .minute = 0 };
    unsigned short workTimeShift = 5; // minutes

    unsigned short breaksBetweenChecks = 60; // minutes
    unsigned short numberOfChecks = 10;
    unsigned short checkingFrequency = 1; // seconds
    unsigned short minNumberOfUnsuccessfullAttempts = 70; // %
    unsigned short waterAdditionTimeout = 5 * 60; // seconds
    unsigned short addionalWaterTankRefillTimeout = 30 * 60; // seconds
};
