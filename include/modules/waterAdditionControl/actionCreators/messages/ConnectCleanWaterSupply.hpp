#pragma once

#include "../../../../utils/informationBanner/AvoidableInformationBannerActionCreator.hpp"

AvoidableInformationBannerActionCreator* connectCleanWaterSupplyMessage(ActionCreator* target = nullptr) {
    return new AvoidableInformationBannerActionCreator(target, 0, true,
        "   Podlacz zrodlo",
        "    czystej wody",
        "",
        "< anuluj    gotowe >"
    );
}