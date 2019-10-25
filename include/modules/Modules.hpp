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
#include "servoValvesStandbyControl/Module.hpp"
#include "serviceMode/Module.hpp"


SystemModulesList* getSystemModules(navRoot* navRootDependency, TimeGuard* timeGuard) {
    logln("Initializing system dependencies");

    logln("Initializing relayModule")
    RelayModule* relayModule = new RelayModule(6,
        new unsigned short[6] { mainPumpPin,    addionalPumpPin,    aerationPin,    sterilizationPin,   lightingPin,    heaterPin },
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
        NUMBER_OF_VALVES,
        SERVOS_OPEN_ANGLE,
        SERVOS_CLOSE_ANGLE, 
        SERVOS_OPEN_CLOSE_TIMEOUT,
        REMOTES_DETECTION_PIN,

        // servo pwm pinout
        new unsigned short[NUMBER_OF_VALVES] {
            AQUARIUM_WATER_VALVE_PIN,
            ADDIONAL_WATER_TANK_VALVE_PIN,
            CLEAN_WATER_VALVE_PIN,
            SEWAGE_WATER_VALVE_PIN
        },

        // servo close detection pinout
        new unsigned short[NUMBER_OF_VALVES] {
            AQUARIUM_WATER_VALVE_CLOSE_DETECTION_PIN,
            ADDIONAL_WATER_TANK_VALVE_CLOSE_DETECTION_PIN,
            CLEAN_WATER_VALVE_CLOSE_DETECTION_PIN,
            SEWAGE_WATER_VALVE_CLOSE_DETECTION_PIN
        }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule(timeGuard);
    FeedingControlModule* feedingControlModule = new FeedingControlModule(relayModule);
    WaterChangeModule* waterChangeModule = new WaterChangeModule(relayModule, waterLevelSensor, valveModule);
    WaterAdditionControlModule* waterAdditionControlModule = new WaterAdditionControlModule(relayModule, waterLevelSensor, valveModule);
    LightingControlModule* lightingControlModule = new LightingControlModule(relayModule);
    AerationControlModule* aerationControlModule = new AerationControlModule(relayModule);
    HeaterControlModule* heaterControlModule = new HeaterControlModule(relayModule);
    SterilizationControlModule* sterilizationControlModule = new SterilizationControlModule(relayModule);
    ServoValvesStandbyControlModule* servoValvesStandbyControlModule = new ServoValvesStandbyControlModule(valveModule);
    ServiceModeModule* serviceModeModule = new ServiceModeModule(waterLevelSensor, relayModule, valveModule, navRootDependency);

    
    #define NUMBER_OF_MODULES 10

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        feedingControlModule,
        waterChangeModule,
        waterAdditionControlModule,
        lightingControlModule,
        aerationControlModule,
        heaterControlModule,
        sterilizationControlModule,
        servoValvesStandbyControlModule,
        serviceModeModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}