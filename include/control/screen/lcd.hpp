#pragma once

#include <LiquidCrystal_PCF8574.h>

#include "customCharset.hpp"

LiquidCrystal_PCF8574 lcd(0x27);

void initializeLcd() {
    lcd.begin(20, 4);
    lcd.setBacklight(255);

    lcd.createChar(arrowTopCustomChar, arrowTopCharDefinition);
    lcd.createChar(arrowDownCustomChar, arrowDownCharDefinition);
    lcd.createChar(arrowLeftCustomChar, arrowLeftCharDefinition);
    lcd.createChar(arrowRightCustomChar, arrowRightCharDefinition);
    lcd.createChar(doubleVerticalArrowCustomChar, doubleVerticalArrowCharDefinition);
}

void printProgramHeader(LiquidCrystal_PCF8574* lcd) {
    lcd->clear();
    lcd->setCursor(0, 1);
    lcd->print("     Kontroler      ");
    lcd->setCursor(0, 2);
    lcd->print("   Akwarystyczny    ");
}