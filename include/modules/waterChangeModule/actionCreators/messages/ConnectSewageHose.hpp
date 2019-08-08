#pragma once

#include "../../../../utils/informationBanner/AvoidableInformationBannerActionCreator.hpp"

AvoidableInformationBannerActionCreator* ConnectSewageHose(ActionCreator* nextTarget) {
    return new AvoidableInformationBannerActionCreator(nextTarget, 5 * 60, true,
        "   Podmiana wody",
        "    Podlacz waz",
        "     sciekowy",
        "< anuluj    gotowe >"
    );
}