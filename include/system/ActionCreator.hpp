#pragma once

#include <RtcDS1302.h>
#include <LiquidCrystal_PCF8574.h>

#include "../control/joystick/JoystickActions.hpp"

class ActionCreator {
    public:
        virtual void setup(LiquidCrystal_PCF8574* lcd) = 0;
        virtual ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) = 0;
};

class CommonActionCreator: public ActionCreator {
    protected:
        LiquidCrystal_PCF8574* lcd;
        virtual void setup() {}
    
    public:
        virtual void setup(LiquidCrystal_PCF8574* lcd) { this->lcd = lcd; setup(); }
};
