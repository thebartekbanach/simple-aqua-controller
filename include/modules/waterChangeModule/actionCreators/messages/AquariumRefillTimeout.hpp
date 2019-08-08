#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* AquariumRefillTimeout(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        " Uwaga: woda wlewa",
        "  sie zbyt dlugo.",
        "",
        "  Sprawdz zawory"
    );
}