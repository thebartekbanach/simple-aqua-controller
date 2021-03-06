#pragma once

/*
    Device IDs needs to have
    number corresponding to
    in how order devices
    are registered in Modules.hpp,
    Device ID means
    for example:
        mainPump >0<
    is registered first
    so ID is 0
*/

#define NUMBER_OF_RELAYS 6

#define mainPump 0
#define MAIN_PUMP_PIN 44

#define addionalPump 1
#define ADDIONAL_PUMP_PIN 45

#define aeration 2
#define AERATION_PIN 46

#define sterilization 3
#define STERILIZATION_PIN 47

#define heatingLamp 4
#define HEATING_LAMP_PIN 48

#define heater 5
#define HEATER_PIN 7

#define ON true
#define OFF false