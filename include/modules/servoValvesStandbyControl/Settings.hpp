#pragma once

struct ServoValvesStandbyControlModuleSettings {
    bool standbyControlActive = true;
    unsigned short standbyTimeout = 10; // seconds
};