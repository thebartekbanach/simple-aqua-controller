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

        static action forCb(TClass* obj, void (TClass::*method)()) {
            return action(new ActionReceiver<TClass>(obj, method));
        } 
};

template<class TClass>
class ExactActionReceiver: public actionReceiver {
    private:
        TClass* obj = nullptr;
        void (TClass::*method)(FUNC_PARAMS) = nullptr;

    public:
        ExactActionReceiver(TClass* obj, void (TClass::*method)(FUNC_PARAMS)):
            obj(obj), method(method) {}

        void call(FUNC_PARAMS) override {
            (obj->*method)(FUNC_VALUES);
        }
        
        static action forCb(TClass* obj, void (TClass::*method)(FUNC_PARAMS)) {
            return action(new ExactActionReceiver<TClass>(obj, method));
        } 
};