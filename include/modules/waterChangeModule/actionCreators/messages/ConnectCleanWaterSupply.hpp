#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* ConnectCleanWaterSupply(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "   Podmiana wody",
        "  Podlacz zrodlo",
        "   czystej wody",
        "           gotowe >"
    );
}