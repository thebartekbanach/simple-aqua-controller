#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillCompleteMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 30, true,
        "  Woda w zbiorniku",
        " rezerwowym zostala",
        "    uzupelniona!",
        " Odlacz zrodlo wody"
    );
}