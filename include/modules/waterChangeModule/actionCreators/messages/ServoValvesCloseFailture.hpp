#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* ServoValvesCloseFailture(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "Uwaga! Nie udalo sie",
        " zamknac wszystkich",
        "    serwozaworow",
        "    Sprawdz to!"
    );
}