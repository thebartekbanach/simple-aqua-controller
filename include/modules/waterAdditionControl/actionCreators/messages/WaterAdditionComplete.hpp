#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterAdditionCompleteMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 10, true,
        "",
        " Poziom wody zostal",
        "    uzupelniony"
    );
}