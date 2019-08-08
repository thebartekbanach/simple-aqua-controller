#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* AquariumWaterOutletTimeout(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "   Uwaga: Timeout",
        "   usuwania wody",
        "    z akwarium.",
        " Sprawdz wylew wody"
    );
}
