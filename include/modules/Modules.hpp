#pragma once

#include "../program/TimeGuard.hpp"

#include "../system/SystemModulesList.hpp"

#include "../control/relayModule/RelayModule.hpp"
#include "../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../control/valves/ValveModule.hpp"

#include "timeSetup/Module.hpp"
#include "feedingControl/Module.hpp"
#include "waterChangeModule/Module.hpp"
#include "waterAdditionControl/Module.hpp"
#include "aerationControl/Module.hpp"
#include "lightingControl/Module.hpp"
#include "heaterControl/Module.hpp"
#include "sterilizationControl/Module.hpp"
#include "serviceMode/Module.hpp"

SystemModulesList* getSystemModules(navRoot* navRootDependency, TimeGuard* timeGuard) {
    logln("Initializing system dependencies");

    logln("Initializing relayModule")
    RelayModule* relayModule = new RelayModule(6,
        new ushort[6] { mainPumpPin,    addionalPumpPin,    aerationPin,    sterilizationPin,   lightingPin,    heaterPin },
        new bool[6] {   HIGH,           HIGH,               HIGH,           HIGH,               LOW,            LOW }
    );
    
    logln("Initializing waterLevelSensor")
    WaterLevelSensor* waterLevelSensor = new WaterLevelSensor((short unsigned int)2, new short unsigned int[2] { 2,  2 }, 
        new short unsigned int*[2] {
            new short unsigned int[2] { // aquariumWater
                changeWaterLevelPin,
                normalWaterLevelPin
            },
            new short unsigned int[2] { // addionalWaterTank
                addionalWaterTankMaxLevelPin,
                addionalWaterTankMinLevelPin
            }
        }
    );

    logln("Initializing valveModule")
    ValveModule* valveModule = new ValveModule(
        4, 0, 180, 2000, 40,
        new unsigned short[4] { 3, 4, 5, 6 },
        new unsigned short[4] { 33, 35, 37, 39 }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule(timeGuard);
    FeedingControlModule* feedingControlModule = new FeedingControlModule(relayModule);
    WaterChangeModule* waterChangeModule = new WaterChangeModule(waterLevelSensor, valveModule);
    WaterAdditionControlModule* waterAdditionControlModule = new WaterAdditionControlModule(relayModule, waterLevelSensor, valveModule);
    AerationControlModule* aerationControlModule = new AerationControlModule(relayModule);
    LightingControlModule* lightingControlModule = new LightingControlModule(relayModule);
    HeaterControlModule* heaterControlModule = new HeaterControlModule(relayModule);
    SterilizationControlModule* sterilizationControlModule = new SterilizationControlModule(relayModule);
    ServiceModeModule* serviceModeModule = new ServiceModeModule(waterLevelSensor, relayModule, valveModule, navRootDependency);

    
    #define NUMBER_OF_MODULES 9

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        feedingControlModule,
        waterChangeModule,
        waterAdditionControlModule,
        aerationControlModule,
        lightingControlModule,
        heaterControlModule,
        sterilizationControlModule,
        serviceModeModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}