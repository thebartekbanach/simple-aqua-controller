#pragma once

#include "../system/SystemModulesList.hpp"

#include "../control/relayModule/RelayModule.hpp"

#include "timeSetup/Module.hpp"
#include "mainPump/Module.hpp"
#include "aerationControl/Module.hpp"
#include "lightingControl/Module.hpp"
#include "heatherControl/Module.hpp"
#include "sterilizationControl/Module.hpp"

SystemModulesList* getSystemModules() {
    logln("Initializing system dependencies");

    RelayModule* relayModule = new RelayModule(8,
        new ushort[8] { mainPumpPin,    addionalPumpPin,    heatherPin,     aerationPin,    sterilizationPin,   lightingPin,    40,     38 },
        new bool[8] {   LOW,            HIGH,               HIGH,           HIGH,           HIGH,               HIGH,           HIGH,   HIGH }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule();
    MainPumpModule* mainPumpModule = new MainPumpModule(relayModule);
    AerationControlModule* aerationControlModule = new AerationControlModule(relayModule);
    LightingControlModule* lightingControlModule = new LightingControlModule(relayModule);
    HeatherControlModule* heatherControlModule = new HeatherControlModule(relayModule);
    SterilizationControlModule* sterilizationControlModule = new SterilizationControlModule(relayModule);
    
    #define NUMBER_OF_MODULES 6

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        mainPumpModule,
        aerationControlModule,
        lightingControlModule,
        heatherControlModule,
        sterilizationControlModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}