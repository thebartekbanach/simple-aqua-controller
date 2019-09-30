#pragma once

#include <Arduino.h>
#include "JoystickActions.hpp"

#ifndef byte
    #define byte uint8_t
#endif

class JoystickController {
    private:
        const byte xPin;
        const byte yPin;
        const ushort moveDelayTime;

    public:
        JoystickController(const byte &xPin, const byte &yPin,
                        const ushort &moveDelayTime = (ushort)0):
            xPin(xPin), 
            yPin(yPin),
            moveDelayTime(moveDelayTime) {
                pinMode(xPin, INPUT);
                pinMode(yPin, INPUT);
        }

        JoystickActions collectActions() {
            int xVal = analogRead(this->xPin);
            int yVal = analogRead(this->yPin);

            JoystickActions action = NONE;
            
            if (yVal < 10) {
                action = UP;
            } else if (yVal > 1000) {
                action = DOWN;
            } else if (xVal > 1000) {
                action = BACK;
            } else if (xVal < 10) {
                action = OK;
            }

            if (action != NONE) {
                delay(moveDelayTime);
            }

            return action;
        }
};
