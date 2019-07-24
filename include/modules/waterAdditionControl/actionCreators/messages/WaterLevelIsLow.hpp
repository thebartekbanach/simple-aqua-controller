#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterLevelIsLowMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "Wykryto niski poziom",
        "wody w zbiorniku.",
        "Uzupelnianie wody",
        "ze zbiornika rezerw."
    );
}