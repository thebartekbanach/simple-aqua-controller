#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* DisconnectCleanWaterSupply(ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 0, true,
        "   Podmiana wody",
        "    zakonczona",
        "    Odlacz wode",
        "          gotowe >"
    );
}