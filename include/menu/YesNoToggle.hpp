#pragma once

#include "TwoWayToggle.hpp"

toggle<bool>* YesNoToggle(char* name, bool& target, actionReceiver* updateAction) {
    return TwoWayToggle<bool>(name, target,
        "tak", true,
        "nie", false,
        updateAction
    );
}