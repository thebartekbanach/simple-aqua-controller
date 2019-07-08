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
            
            digitalWrite(modulePins[module], newState == ON ? !turnedOnStates[module] : turnedOnStates[module]); // high = off, low = on
            stateList[module] = newState;
        }

        bool get(const ushort& module) {
            return stateList[module];
        }

};