#pragma once

#include "TwoWayToggle.hpp"

toggle<bool>* OnOffToggle(char* name, bool& target, actionReceiver* updateAction) {
    return TwoWayToggle<bool>(name, target,
        "wl", true,
        "wyl", false,
        updateAction
    );
}