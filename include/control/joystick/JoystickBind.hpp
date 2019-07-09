#pragma once
#include <menu.h>
#include "JoystickActions.hpp"

Menu::navCmds joystickActionToMenuAction(const JoystickActions &action) {
    switch (action)
    {
    case UP:
        return downCmd;
    case DOWN:
        return upCmd;
    case BACK:
        return escCmd;
    case OK:
        return enterCmd;
    default:
        return noCmd;
    }
}