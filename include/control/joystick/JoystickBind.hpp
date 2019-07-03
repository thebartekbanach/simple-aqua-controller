#pragma once
#include <menu.h>
#include "JoystickActions.hpp"

Menu::navCmds joystickActionToMenuAction(const JoystickActions &action) {
    switch (action)
    {
    case UP:
        return upCmd;
    case DOWN:
        return downCmd;
    case BACK:
        return escCmd;
    case OK:
        return enterCmd;
    default:
        return noCmd;
    }
}