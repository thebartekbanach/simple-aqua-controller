#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankEmptyingTimeoutMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 0, true,
        "   Uwaga: timeout",
        " oproznienia wody w",
        "zbiorniku rezerwowym",
        "   Sprawdz zawory"
    );
}