#pragma once

#include "../system/SystemModulesList.hpp"

#include "TestModule.hpp"

SystemModulesList* getSystemModules() {
    TestModule* testModule = new TestModule();
    
    #define NUMBER_OF_MODULES 1

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        testModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}