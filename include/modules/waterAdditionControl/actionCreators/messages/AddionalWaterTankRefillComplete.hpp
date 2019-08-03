#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillCompleteMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 30, true,
        " Zbiornik rezerwowy",
        " zostal uzupelniony",
        " Odlacz zrodlo wody",
        "          > gotowe"
    );
}