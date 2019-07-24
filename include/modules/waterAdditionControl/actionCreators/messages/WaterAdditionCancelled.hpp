#pragma once

#include "../../../../utils/InformationBannerActionCreator.hpp"

InformationBannerActionCreator* waterAdditionCancelledMessage(ActionCreator* target = nullptr) {
    return new InformationBannerActionCreator(target, 5, true,
        "Anulowanie dolewki",
        "wody. Nastepny cykl",
        "automatycznie",
        "uzupelni wode."
    );
}