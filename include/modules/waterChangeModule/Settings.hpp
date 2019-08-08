#pragma once

struct WaterChangeModuleSettings {
    bool refillAddionalWaterTankDuringWaterChange = true;

    float aquariumWaterOutflowTimeout = 5; // minutes
    float aquariumRefillTimeout = 5; // minutes
    float addionalWaterTankRefillTimeout = 5; // minutes
};
