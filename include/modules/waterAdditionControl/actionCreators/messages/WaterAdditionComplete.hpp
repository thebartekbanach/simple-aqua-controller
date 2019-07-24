#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterAdditionCompleteMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "",
        " Poziom wody zostal",
        "     uzupelniony."
    );
}