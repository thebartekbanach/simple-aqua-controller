#pragma once

#include "../DeviceWorkingMode.hpp"
#include "../../system/SystemTime.hpp"

struct TimeScopedDeviceDriverSettings {
    TimeScopedDeviceDriverSettings(DeviceWorkingMode mode = CONTINUOUS_OFF):
        mode(mode) {}

    TimeScopedDeviceDriverSettings(
        const SystemTime start,
        const SystemTime end,
        DeviceWorkingMode mode = CONTINUOUS_OFF
    ): start(start), end(end), mode(mode) {}

    DeviceWorkingMode mode = DeviceWorkingMode::CONTINUOUS_OFF;
    SystemTime start = { .hour = 0, .minute = 0 };
    SystemTime end = { .hour = 0, .minute = 0 };
};
