#pragma once

#include "../../utils/SystemTime.hpp"

struct MainPumpModuleSettings {
    SystemTime feedingLength = { .hour = 0, .minute = 15 };
};
