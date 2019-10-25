#pragma once

/*
    Base servo valves configuration
*/

#define NUMBER_OF_VALVES 4
#define SERVOS_OPEN_ANGLE 0
#define SERVOS_CLOSE_ANGLE 180
#define SERVOS_OPEN_CLOSE_TIMEOUT 3000

/*
    Servo valves name
    definitions in order
    to how valves are registered
    in Modules.hpp file
*/

#define unknownValve -1
#define aquariumWaterValve 0
#define addionalWaterTankValve 1
#define cleanWaterValve 2
#define sewageWaterValve 3

/*
    Valves servos pwm pinout definition
*/

#define AQUARIUM_WATER_VALVE_PIN 3
#define ADDIONAL_WATER_TANK_VALVE_PIN 4
#define CLEAN_WATER_VALVE_PIN 5
#define SEWAGE_WATER_VALVE_PIN 6
#define REMOTES_DETECTION_PIN 40

/*
    Valves close detection pinout definition
*/

#define AQUARIUM_WATER_VALVE_CLOSE_DETECTION_PIN 33
#define ADDIONAL_WATER_TANK_VALVE_CLOSE_DETECTION_PIN 35
#define CLEAN_WATER_VALVE_CLOSE_DETECTION_PIN 37
#define SEWAGE_WATER_VALVE_CLOSE_DETECTION_PIN 39
