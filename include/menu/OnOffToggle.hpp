#pragma once

#include "TwoWayToggle.hpp"

toggle<bool>* OnOffToggle(char* name, bool& target, actionReceiver* updateAction, eventMask event = exitEvent) {
    return TwoWayToggle<bool>(name, target,
        "wl", true,
        "wyl", false,
        updateAction,
        event
    );
}