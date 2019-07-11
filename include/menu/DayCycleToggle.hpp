#pragma once

#include <menu.h>

#include "../modules/timeSetup/DayCycle.hpp"

prompt* dayCycleToogle(char* fieldName, DayCycle& dayCycle, actionReceiver* updateAction) {
    prompt** toggleValues = new prompt*[3] {
        new menuValue<DayCycle>("manualnym", DayCycle::UNSET),
        new menuValue<DayCycle>("dnia", DayCycle::DAY),
        new menuValue<DayCycle>("nocy", DayCycle::NIGHT)
    };

    prompt* toogle = new toggle<DayCycle>(fieldName, dayCycle, 3, toggleValues, action(updateAction), exitEvent, wrapStyle);

    return toogle;
}