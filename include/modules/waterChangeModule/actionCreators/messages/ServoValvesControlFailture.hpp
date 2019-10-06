#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* ServoValvesControlFailture(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "       Uwaga!",
        "Niektore serwozawory",
        "moga byc uszkodzone!",
        "    Sprawdz to!"
    );
}