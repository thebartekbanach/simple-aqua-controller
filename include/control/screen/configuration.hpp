#pragma once

#include <Arduino.h>
#include <menuIO/chainStream.h>
#include <menuIO/PCF8574Out.h>
#include "lcd.hpp"

#define MAX_DEPTH 3

chainStream<0> menuInput(nullptr);

MENU_OUTPUTS(menuOutput, MAX_DEPTH,
    LCD_OUT(lcd, { 0, 0, 20, 4 }),
    NONE
);