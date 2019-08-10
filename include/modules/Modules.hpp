#pragma once

#include "../program/TimeGuard.hpp"

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
#include "serviceMode/Module.hpp"

SystemModulesList* getSystemModules(navRoot* navRootDependency, TimeGuard* timeGuard) {
    logln("Initializing system dependencies");

    RelayModule* relayModule = new RelayModule(8,
        new ushort[8] { mainPumpPin,    addionalPumpPin,    heatherPin,     aerationPin,    sterilizationPin,   lightingPin,    addionalTankValvePin,   aquariumValvePin },
        new bool[8] {   LOW,            HIGH,               HIGH,           HIGH,           HIGH,               HIGH,           HIGH,                   HIGH }
    );
    
    WaterLevelSensor* waterLevelSensor = new WaterLevelSensor((short unsigned int)2, new short unsigned int[2] { 2,  2 }, 
        new short unsigned int*[2] {
            new ushort[2] { // aquariumWater
                changeWaterLevelPin,
                normalWaterLevelPin
            },
            new short unsigned int[2] { // addionalWaterTank
                addionalWaterTankMaxLevelPin,
                addionalWaterTankMinLevelPin
            }
        }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule(timeGuard);
    FeedingControlModule* feedingControlModule = new FeedingControlModule(relayModule);
    WaterChangeModule* waterChangeModule = new WaterChangeModule(waterLevelSensor, relayModule);
    WaterAdditionControlModule* waterAdditionControlModule = new WaterAdditionControlModule(relayModule, waterLevelSensor);
    AerationControlModule* aerationControlModule = new AerationControlModule(relayModule);
    LightingControlModule* lightingControlModule = new LightingControlModule(relayModule);
    HeatherControlModule* heatherControlModule = new HeatherControlModule(relayModule);
    SterilizationControlModule* sterilizationControlModule = new SterilizationControlModule(relayModule);
    ServiceModeModule* serviceModeModule = new ServiceModeModule(waterLevelSensor, relayModule, navRootDependency);

    
    #define NUMBER_OF_MODULES 9

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        feedingControlModule,
        waterChangeModule,
        waterAdditionControlModule,
        aerationControlModule,
        lightingControlModule,
        heatherControlModule,
        sterilizationControlModule,
        serviceModeModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}