#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankLevelIsToLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "Zbyt niski poziom",
        "wody w zbiorniku",
        "rezerwowym.",
        "Uzupelnij wode!"
    );
}