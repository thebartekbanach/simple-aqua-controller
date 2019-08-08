#pragma once

#include "../../../../utils/informationBanner/AvoidableInformationBannerActionCreator.hpp"

AvoidableInformationBannerActionCreator* ConnectHose(ActionCreator* nextTarget) {
    return new AvoidableInformationBannerActionCreator(nextTarget, 0, true,
        "Reczna podmiana wody",
        "   Podlacz waz do",
        "  wody lub sciekow ",
        "< anuluj    gotowe >"
    );
}