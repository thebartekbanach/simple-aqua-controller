#pragma once

#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);

void initializeLcd() {
    lcd.begin(20, 4);
    lcd.setBacklight(255);
}

void printProgramHeader(LiquidCrystal_PCF8574* lcd) {
    lcd->clear();
    lcd->setCursor(0, 1);
    lcd->print("     Kontroler      ");
    lcd->setCursor(0, 2);
    lcd->print("   Akwarystyczny    ");
}