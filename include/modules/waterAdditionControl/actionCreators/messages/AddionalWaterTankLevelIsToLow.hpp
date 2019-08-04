#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankLevelIsToLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 30, true,
        "",
        "Niski poziom rezerwy",
        " Uzupelnij rezerwe"
    );
}