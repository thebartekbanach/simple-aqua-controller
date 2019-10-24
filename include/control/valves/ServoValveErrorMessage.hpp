#pragma once

#include "../../utils/informationBanner/InformationBannerActionCreator.hpp"

#include "devices.hpp"

InformationBannerActionCreator* servoValveErrorMessage(unsigned short valveId, ActionCreator* target = nullptr) {
    char* name = nullptr;
    
    switch (valveId) {
        case aquariumWaterValve:        name = "     akwarium";         break;
        case addionalWaterTankValve:    name = "     rezerwy";          break;
        case cleanWaterValve:           name = "   czystej wody";       break;
        case sewageWaterValve:          name = "   brudnej wody";       break;
        default:                        name = "";                      break;
    }

    if (name == "") return new InformationBannerActionCreator(target, 0, true,
        "   Blad krytyczny",
        "",
        "    Zablokowany",
        "    serwozawor"
    );

    else return new InformationBannerActionCreator(target, 0, true,
        "   Blad krytyczny",
        "    Zablokowany",
        "    serwozawor:",
        name
    );
}