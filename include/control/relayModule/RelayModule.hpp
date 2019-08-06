#pragma once

#include <Arduino.h>

#include "devices.hpp"

class RelayModule {
    private:
        const ushort* modulePins;
        const bool* turnedOffStates;
        const ushort numberOfModules;
        bool* stateList;

        void initialize() {
            for (ushort i = 0; i < numberOfModules; ++i) {
                pinMode(modulePins[i], OUTPUT);
                digitalWrite(modulePins[i], turnedOffStates[i]);
            }
        }

    public:
        RelayModule(const ushort numberOfModules, const ushort* modulePins, const bool* turnedOffStates):
            modulePins(modulePins),
            turnedOffStates(turnedOffStates),
            numberOfModules(numberOfModules),
            stateList(new bool[numberOfModules] { false }) {
                initialize();
            }

        void set(const ushort& module, const bool& newState) {
            if (module >= numberOfModules || module < 0) return;
            if (stateList[module] == newState) return;
            
            log("SETTING MOD: ") log(module) log(" TO ") logln(newState);
            digitalWrite(modulePins[module], newState == ON ? !turnedOffStates[module] : turnedOffStates[module]); // high = off, low = on

            delay(300); // electromagnetic interference from relay module...

            stateList[module] = newState;
        }

        bool get(const ushort& module) {
            return stateList[module];
        }

};
