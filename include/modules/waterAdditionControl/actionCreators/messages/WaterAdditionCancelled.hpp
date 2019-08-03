#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterAdditionCancelledMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 20, true,
        "Dolewka wody zostala",
        "      anulowana",
        "Woda zostanie dolana",
        " w nastepnym cyklu"
    );
}