#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterLevelIsEnoughtMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "",
        "  Poziom wody jest",
        "wystarczajaco wysoki"
    );
}