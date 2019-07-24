#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankLevelIsToLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "Wykryto zbyt niski",
        "poziom wody w",
        "zbiorniku rezerwowym.",
        "Trzeba uzupełnic."
    );
}