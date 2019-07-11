#pragma once

#include <menu.h>

#include "../utils/DeviceWorkingMode.hpp"

prompt* turnStateToggle(char* fieldName, DeviceWorkingMode& dayCycle, actionReceiver* updateAction) {
    prompt** toggleValues = new prompt*[5] {
        new menuValue<DeviceWorkingMode>("wylaczony", DeviceWorkingMode::CONTINUOUS_OFF),
        new menuValue<DeviceWorkingMode>("wlaczony", DeviceWorkingMode::CONTINUOUS_ON),
        new menuValue<DeviceWorkingMode>("dzienny", DeviceWorkingMode::IN_DAY_CYCLE),
        new menuValue<DeviceWorkingMode>("nocny", DeviceWorkingMode::IN_NIGHT_CYCLE),
        new menuValue<DeviceWorkingMode>("w zakresie", DeviceWorkingMode::FROM_SETTINGS)
    };

    prompt* toogle = new toggle<DeviceWorkingMode>(fieldName, dayCycle, 5, toggleValues, action(updateAction), enterEvent, wrapStyle);

    return toogle;
}