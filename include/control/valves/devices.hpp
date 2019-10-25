#pragma once

/*
    Base servo valves configuration
*/

#define NUMBER_OF_VALVES (unsigned short) 4
#define SERVOS_OPEN_ANGLE (unsigned short) 0
#define SERVOS_CLOSE_ANGLE (unsigned short) 180
#define SERVOS_OPEN_CLOSE_TIMEOUT (unsigned short) 3000

/*
    Servo valves name
    definitions in order
    to how valves are registered
    in Modules.hpp file
*/

#define unknownValve (short) -1
#define aquariumWaterValve (short) 0
#define addionalWaterTankValve (short) 1
#define cleanWaterValve (short) 2
#define sewageWaterValve (short) 3

/*
    Valves servos pwm pinout definition
*/

#define AQUARIUM_WATER_VALVE_PIN (unsigned short) 3
#define ADDIONAL_WATER_TANK_VALVE_PIN (unsigned short) 4
#define CLEAN_WATER_VALVE_PIN (unsigned short) 5
#define SEWAGE_WATER_VALVE_PIN (unsigned short) 6
#define REMOTES_DETECTION_PIN (unsigned short) 40

/*
    Valves close detection pinout definition
*/

#define AQUARIUM_WATER_VALVE_CLOSE_DETECTION_PIN (unsigned short) 33
#define ADDIONAL_WATER_TANK_VALVE_CLOSE_DETECTION_PIN (unsigned short) 35
#define CLEAN_WATER_VALVE_CLOSE_DETECTION_PIN (unsigned short) 37
#define SEWAGE_WATER_VALVE_CLOSE_DETECTION_PIN (unsigned short) 39
