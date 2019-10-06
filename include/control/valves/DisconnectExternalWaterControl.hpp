#pragma once

#include "../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* DisconnectExternalWaterControl(String title, ActionCreator* nextTarget) {
    return new InformationBannerActionCreator(nextTarget, 3600, true,
        title,
        "  Odlacz sterowane",
        "zlacze wody i scieku",
        "          zrobione >"
    );
}