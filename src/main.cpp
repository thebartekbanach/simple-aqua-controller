#include <Arduino.h>

#define DEBUG_MODE_ENABLED
#include "utils/log.hpp"

#include "diagnostics/WaterLevelSensor.hpp"
#include "diagnostics/I2cScanner.hpp"

#ifndef DIAGNOSTICS_MODE

#include "program/Program.hpp"

Program* program;

void setup () 
{
    DEBUG_SERIAL_OUTPUT.begin(115200);

    logln("Enabling watchdog");
    watchdogEnable(1000);

    logln("Starting program...");
    program = new Program();

    logln("Starting setup...");
    program->setup();

    logln("Program started!");
}

//void watchdogSetup() {
    // watchdog enable: https://forum.arduino.cc/index.php?topic=350058.msg2416416#msg2416416
//}

void loop () 
{
    //watchdogReset();
    program->update();
}

#endif