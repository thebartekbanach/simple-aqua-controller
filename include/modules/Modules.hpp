#pragma once

#include "../system/SystemModulesList.hpp"

#include "../control/relayModule/RelayModule.hpp"
#include "../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "timeSetup/Module.hpp"
#include "feedingControl/Module.hpp"
#include "waterChangeModule/Module.hpp"
#include "waterAdditionControl/Module.hpp"
#include "aerationControl/Module.hpp"
#include "lightingControl/Module.hpp"
#include "heatherControl/Module.hpp"
#include "sterilizationControl/Module.hpp"

SystemModulesList* getSystemModules() {
    logln("Initializing system dependencies");

    RelayModule* relayModule = new RelayModule(8,
        new ushort[8] { mainPumpPin,    addionalPumpPin,    heatherPin,     aerationPin,    sterilizationPin,   lightingPin,    addionalTankValvePin,   aquariumValvePin },
        new bool[8] {   LOW,            HIGH,               HIGH,           HIGH,           HIGH,               HIGH,           HIGH,                   HIGH }
    );
    
    WaterLevelSensor* waterLevelSensor = new WaterLevelSensor((short unsigned int)2, new short unsigned int[2] { 2,  2 }, 
        new short unsigned int*[2] {
            new ushort[2] {
                changeWaterLevelPin,
                normalWaterLevelPin
            },
            new short unsigned int[2] {
                addionalWaterTankMaxLevelPin,
                addionalWaterTankMinLevelPin
            }
        }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule();
    FeedingControlModule* feedingControlModule = new FeedingControlModule(relayModule);
    WaterChangeModule* waterChangeModule = new WaterChangeModule(waterLevelSensor, relayModule);
    WaterAdditionControlModule* waterAdditionControlModule = new WaterAdditionControlModule(relayModule, waterLevelSensor);
    AerationControlModule* aerationControlModule = new AerationControlModule(relayModule);
    LightingControlModule* lightingControlModule = new LightingControlModule(relayModule);
    HeatherControlModule* heatherControlModule = new HeatherControlModule(relayModule);
    SterilizationControlModule* sterilizationControlModule = new SterilizationControlModule(relayModule);

    
    #define NUMBER_OF_MODULES 8

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        feedingControlModule,
        waterChangeModule,
        waterAdditionControlModule,
        aerationControlModule,
        lightingControlModule,
        heatherControlModule,
        sterilizationControlModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}