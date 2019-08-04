#pragma once

#include "../../../../utils/informationBanner/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterLevelIsLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 30, true,
        "Wykryto niski poziom",
        "  wody w akwarium",
        " Uzupelnianie wody",
        "ze zbiornika rezerw."
    );
}