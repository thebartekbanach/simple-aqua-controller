#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterLevelIsLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 30, true,
        "Wykryto niski poziom",
        "  wody w zbiorniku",
        " Uzupelnianie wody",
        "ze zbiornika rezerw."
    );
}