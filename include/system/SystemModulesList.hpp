#pragma once

#include "SystemModule.hpp"

struct SystemModulesList {
    SystemModule **items;
    unsigned short length;

    SystemModulesList(SystemModule* items[], const unsigned short &length):
        items(items), length(length) {}
};
