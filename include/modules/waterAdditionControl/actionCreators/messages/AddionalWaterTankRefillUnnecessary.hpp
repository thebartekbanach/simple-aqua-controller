#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillUnnecessaryMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 10, true,
        "",
        " Zbiornik rezerwowy",
        "   jest juz pelen"
    );
}