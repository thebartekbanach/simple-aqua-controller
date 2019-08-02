#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterAdditionTimeoutMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 0, true,
        "Uwaga: timeout pompy",
        "dolewki! Sprawdz",
        "pompe, byc moze sie",
        "zatkala lub zepsula"
    );
}