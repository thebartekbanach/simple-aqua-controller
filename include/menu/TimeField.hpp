#pragma once

#include <menu.h>

#include "../system/SystemTime.hpp"

menuNode* timeField(char* title, unsigned short& hours, unsigned short& minutes, actionReceiver* updateEvent, eventMask onEvent = exitEvent) {
    prompt** timePadData = new prompt*[2] {
        new menuField<unsigned short>(hours, "Godzina", "", 0, 23, 1, 0, action(updateEvent), onEvent),
        new menuField<unsigned short>(minutes, "Minuta", "", 0, 59, 1, 0, action(updateEvent), onEvent)
    };

    return new menuNode(title, 2, timePadData);
}

menuNode* timeField(char* title, SystemTime& time, actionReceiver* updateEvent, eventMask onEvent = exitEvent) {
    return timeField(title, time.hour, time.minute, updateEvent, onEvent);
}
