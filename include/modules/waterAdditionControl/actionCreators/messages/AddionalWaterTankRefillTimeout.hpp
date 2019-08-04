#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillTimeoutMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 0, true,
        "   Uwaga: timeout",
        "uzupelnienia wody w",
        "zbiorniku rezerwowym",
        "   Sprawdz zawory"
    );
}