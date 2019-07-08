#pragma once

#include "../system/SystemModulesList.hpp"

#include "TestModule.hpp"
#include "timeSetup/TimeSetupModule.hpp"

SystemModulesList* getSystemModules() {
    TestModule* testModule = new TestModule();
    TimeSetupModule* timeSetup = new TimeSetupModule();
    
    #define NUMBER_OF_MODULES 2

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        testModule,
        timeSetup
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}