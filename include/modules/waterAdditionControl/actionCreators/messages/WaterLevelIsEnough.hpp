#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterLevelIsEnoughtMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 10, true,
        "",
        "  Poziom wody jest",
        "wystarczajaco wysoki"
    );
}