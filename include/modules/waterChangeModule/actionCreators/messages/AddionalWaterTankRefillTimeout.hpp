#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* AddionalWaterTankRefillTimeout(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "   Uwaga: uzupel.",
        "  wody w zbiorniku",
        "  rezerwowym trwa",
        "    zbyt dlugo"
    );
}