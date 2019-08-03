#pragma once

#include "../../system/ActionCreator.hpp"
#include "../../system/SystemTime.hpp"
#include "../../system/Timer.hpp"

class InformationBannerActionCreator: public CommonActionCreator {
    private:
        ActionCreator* target;
        String* text;
        bool canSkip;
        bool canTimeout;
        
        bool initialized = false;
        ushort showTimeInSeconds;
        Timer end;

    public:
        InformationBannerActionCreator(ActionCreator* nextTarget, ushort showTimeInSeconds, bool canSkip,
            String firstLine,
            String secondLine = "",
            String thirdLine = "",
            String fourthLine = ""):
            InformationBannerActionCreator(nextTarget, showTimeInSeconds, canSkip, new String[4] {
                firstLine, secondLine, thirdLine, fourthLine
            }) {}

        InformationBannerActionCreator(ActionCreator* nextTarget, ushort showTimeInSeconds, bool canSkip, String* text):
            target(nextTarget), showTimeInSeconds(showTimeInSeconds), canSkip(canSkip), text(text), canTimeout(showTimeInSeconds != 0) {
                if (!canSkip && !canTimeout) canSkip = true;
            }

        ~InformationBannerActionCreator() {
            delete[] text;
        }

        void setup() {
            lcd->clear();
            
            for (ushort i = 0; i < 4; ++i) {
                lcd->setCursor(0, i);
                lcd->write(text[i].c_str());
            }
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (canTimeout && !initialized) {
                end.start(time, showTimeInSeconds);
                initialized = true;
            }

            if (canTimeout && end.isReached(time)) {
                return target;
            }

            if (canSkip && action == OK) {
                return target;
            }

            return this;
        }
};
