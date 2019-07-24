#pragma once

#include <Arduino.h>

#ifndef DEBUG_SERIAL_OUTPUT
    #define DEBUG_SERIAL_OUTPUT SerialUSB
#endif

#ifdef DEBUG_MODE_ENABLED
    #define log(message) DEBUG_SERIAL_OUTPUT.print(message); DEBUG_SERIAL_OUTPUT.flush();
    #define logln(message) DEBUG_SERIAL_OUTPUT.println(message); DEBUG_SERIAL_OUTPUT.flush();
#endif

#ifndef DEBUG_MODE_ENABLED
    #define log(message)
    #define logln(message)
#endif
