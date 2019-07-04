#pragma once

#include "menu.h"

template<class TClass>
class ActionReceiver: public actionReceiver {
    private:
        TClass* obj = nullptr;
        void (TClass::*method)() = nullptr;

    public:
        ActionReceiver(TClass* obj, void (TClass::*method)()):
            obj(obj), method(method) {}

        void call(FUNC_PARAMS) override {
            (obj->*method)();
        }
};

template<class TClass>
action receiverFor(TClass* obj, void (TClass::*method)()) {
    return action(new ActionReceiver<TClass>(obj, method));
}
