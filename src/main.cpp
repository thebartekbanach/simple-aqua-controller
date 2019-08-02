#include <Arduino.h>

#define DEBUG_MODE_ENABLED
#include "utils/log.hpp"

#include "diagnostics/WaterLevelSensor.hpp"

#ifndef DIAGNOSTICS_MODE

#include "program/Program.hpp"

Program* program;

void setup () 
{
    DEBUG_SERIAL_OUTPUT.begin(115200);

    logln("Starting program...");
    program = new Program();

    logln("Starting setup...");
    program->setup();

    logln("Program started!");
}

void loop () 
{
    program->update();
}

#endif