#pragma once

#include "SystemModule.hpp"

struct SystemModulesList {
    SystemModule **items;
    ushort length;

    SystemModulesList(SystemModule* items[], const ushort &length):
        items(items), length(length) {}
};
