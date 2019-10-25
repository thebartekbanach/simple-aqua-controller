#pragma once

/*
    Base water level sensor configuration
*/

#define NUMBER_OF_WATER_LEVEL_SENSORS (unsigned short) 2
#define NUMBER_OF_AQUARIUM_WATER_SENSORS (unsigned short) 2
#define NUMBER_OF_ADDIONAL_WATER_TANK_SENSORS (unsigned short) 2

/*
    Water level sensor
    device names in order
    to how sensors are registered
    in Modules.hpp file,
    used later as a sensor IDs
*/

#define aquariumWater (unsigned short) 0
#define addionalWaterTank (unsigned short) 1

/*
    Water levels in order to
    how pinout is registered,
    used later as a level IDs
*/

#define normalWaterLevel (unsigned short) 0
#define changeWaterLevel (unsigned short) 1

#define addionalWaterTankMaxLevel (unsigned short) 0
#define addionalWaterTankMinLevel (unsigned short) 1

/*
    Water level detection
    pinout, digital pins
*/

#define CHANGE_WATER_LEVEL_PIN (unsigned short) 28
#define NORMAL_WATER_LEVEL_PIN (unsigned short) 26

#define ADDIONAL_WATER_TANK_MAX_LEVEL_PIN (unsigned short) 27
#define ADDIONAL_WATER_TANK_MIN_LEVEL_PIN (unsigned short) 29