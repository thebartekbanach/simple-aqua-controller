#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankIsAlredyEmpty(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 10, true,
        "",
        "  Zbiornik dolewki",
        "   jest juz pusty"
    );
}