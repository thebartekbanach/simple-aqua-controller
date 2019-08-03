#pragma once

#include "../../system/ActionCreator.hpp"
#include "../../system/SystemTime.hpp"
#include "../../system/Timer.hpp"

#include "InformationBannerActionCreator.hpp"

class AvoidableInformationBannerActionCreator: public InformationBannerActionCreator {
    public:
        AvoidableInformationBannerActionCreator(ActionCreator* nextTarget, ushort showTimeInSeconds, bool canSkip,
            String firstLine,
            String secondLine = "",
            String thirdLine = "",
            String fourthLine = ""):
            InformationBannerActionCreator(nextTarget, showTimeInSeconds, canSkip, new String[4] {
                firstLine, secondLine, thirdLine, fourthLine
            }) {}

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) override {
            if (action == BACK) {
                return nullptr;
            }
            
            return InformationBannerActionCreator::update(time, action);
        }
};
