#pragma once

#include <menu.h>

template<typename T>
toggle<T>* TwoWayToggle(char* name, T& target,
                        char* firstOptionName, const T firstOptionValue,
                        char* secondOptionName, const T secondOptionValue,
                        actionReceiver* updateAction)
{
    prompt** values = new prompt*[2] {
        new menuValue<T>(firstOptionName, firstOptionValue),
        new menuValue<T>(secondOptionName, secondOptionValue)
    };

    return new toggle<T>(name, target, 2, values, action(updateAction), exitEvent, wrapStyle);
}