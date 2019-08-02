#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillCancelledMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 3, true,
        " Uzupelnienie wody",
        "     w zbiorniku",
        " rezerwowym zostalo",
        "      anulowane"
    );
}