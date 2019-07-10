#pragma once

#include <Arduino.h>

#include "devices.hpp"

class RelayModule {
    private:
        const ushort* modulePins;
        const bool* turnedOnStates;
        const ushort numberOfModules;
        bool* stateList;

        void initialize() {
            for (ushort i = 0; i < numberOfModules; ++i) {
                pinMode(modulePins[i], OUTPUT);
                set(i, OFF);
            }
        }

    public:
        RelayModule(const ushort numberOfModules, const ushort* modulePins, const bool* turnedOnStates):
            modulePins(modulePins),
            turnedOnStates(turnedOnStates),
            numberOfModules(numberOfModules),
            stateList(new bool[numberOfModules]) {
                initialize();
            }

        void set(const ushort& module, const bool& newState) {
            if (module >= numberOfModules || module < 0) return;
            if (stateList[module] == newState) return;
            
            digitalWrite(modulePins[module], newState == ON ? !turnedOnStates[module] : turnedOnStates[module]); // high = off, low = on

            delay(300); // electromagnetic interference from relay module...

            stateList[module] = newState;
        }

        bool get(const ushort& module) {
            return stateList[module];
        }

};
