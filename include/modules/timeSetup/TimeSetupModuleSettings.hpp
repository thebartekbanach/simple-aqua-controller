#pragma once

#include "../../utils/SystemTime.hpp"

struct TimeSetupModuleSettings {
    SystemTime dayStart = { .hour = 8, .minute = 00 };
    SystemTime nightStart = { .hour = 20, .minute = 00 };
};
