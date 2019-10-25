#define WATER_LEVEL_SENSOR_DIAGNOSTICS_ENABLED false

#if WATER_LEVEL_SENSOR_DIAGNOSTICS_ENABLED == true
    #define DIAGNOSTICS_MODE

    #include "../utils/log.hpp"

    #include "../control/waterLevelSensor/WaterLevelSensor.hpp"
    #include "../control/waterLevelSensor/devices.hpp"

    WaterLevelSensor* wls = nullptr;

    void setup() {
        wls = new WaterLevelSensor((short unsigned int)2, new short unsigned int[2] { 2,  2 }, 
            new short unsigned int*[2] {
                new unsigned short[2] {
                    changeWaterLevelPin,
                    normalWaterLevelPin
                },
                new short unsigned int[2] {
                    addionalWaterTankMaxLevelPin,
                    addionalWaterTankMinLevelPin
                }
            }
        );
    }

    void loop() {
        auto nwl = wls->sense(aquariumWater, normalWaterLevel);
        auto cwl = wls->sense(aquariumWater, changeWaterLevel);
        auto awtxl = wls->sense(addionalWaterTank, addionalWaterTankMaxLevel);
        auto awtnl = wls->sense(addionalWaterTank, addionalWaterTankMinLevel);

        logln("==============");
        log("Normal water level: ") logln(nwl);
        log("Change water level: ") logln(cwl);
        log("Addional water tank max level: ") logln(awtxl);
        log("Addional water tank min level: ") logln(awtnl);
        
        delay(333);
    }
#endif