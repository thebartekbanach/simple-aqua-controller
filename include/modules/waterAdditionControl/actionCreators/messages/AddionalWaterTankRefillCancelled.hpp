#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* addionalWaterTankRefillCancelledMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 10, true,
        "",
        "Uzupelnienie rezerwy",
        "   wody anulowane"
    );
}