#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* connectCleanWaterSourceMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 0, true,
        "Podlacz zrodlo",
        "czystej wody",
        "",
        "-> zrobione"
    );
}