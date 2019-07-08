#pragma once

#include "../system/SystemModulesList.hpp"

#include "../control/relayModule/RelayModule.hpp"

#include "timeSetup/Module.hpp"
#include "mainPump/Module.hpp"

SystemModulesList* getSystemModules() {
    logln("Initializing system dependencies");

    RelayModule* relayModule = new RelayModule(8,
        new ushort[8] { 52, 50, 48, 46, 44, 42, 40, 38 },
        new bool[8] { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH }
    );

    logln("Initializing system modules")
    
    TimeSetupModule* timeSetupModule = new TimeSetupModule();
    MainPumpModule* mainPumpModule = new MainPumpModule(relayModule);
    
    #define NUMBER_OF_MODULES 2

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        mainPumpModule,
        timeSetupModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}