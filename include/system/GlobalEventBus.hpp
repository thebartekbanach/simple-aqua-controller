#pragma once
#include "SystemModulesList.hpp"

class GlobalEventBus {
    SystemModulesList* modules;

    public:
        GlobalEventBus(SystemModulesList* modules):
            modules(modules) {}
        
        void send(int moduleId, int eventCode, void* data = nullptr) {
            for (unsigned short i = 0; i < modules->length; ++i) {
                modules->items[i]->onEvent(moduleId, eventCode, data);
            }
        }
};
