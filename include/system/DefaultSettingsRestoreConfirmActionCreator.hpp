#pragma once

#include "ActionCreator.hpp"
#include "ActionReceiver.hpp"

#include "../control/screen/customCharset.hpp"

#include "../utils/informationBanner/InformationBannerActionCreator.hpp"

template<class TClass>
class DefaultSettingsRestoreConfirmActionCreator: public CommonActionCreator {
    private:
        TClass* obj = nullptr;
        void (TClass::*method)() = nullptr;

    public:
        DefaultSettingsRestoreConfirmActionCreator(TClass* obj, void (TClass::*method)()):
            obj(obj), method(method) {}

        void setup() override {
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Czy na pewno chcesz");
            lcd->setCursor(0, 1);
            lcd->print("wrocic do ustawien");
            lcd->setCursor(0, 2);
            lcd->print("   fabrycznych?");
            lcd->setCursor(0, 3);
            lcd->print("< anuluj  przywroc >");
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            if (action == BACK) return nullptr;
            
            if (action == OK) {
                (obj->*method)();
                return new InformationBannerActionCreator(nullptr, 10, true,
                    "    Przywrocono",
                    "     ustawienia",
                    "      domyslne",
                    "                ok >"
                );
            }

            return this;
        };
};