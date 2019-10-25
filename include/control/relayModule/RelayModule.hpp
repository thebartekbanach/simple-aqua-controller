#pragma once

#include <Arduino.h>

#include "devices.hpp"

class RelayModule {
    private:
        const unsigned short* modulePins;
        const bool* turnedOffStates;
        const unsigned short numberOfModules;
        bool* stateList;

        void initialize() {
            for (unsigned short i = 0; i < numberOfModules; ++i) {
                pinMode(modulePins[i], OUTPUT);
                digitalWrite(modulePins[i], turnedOffStates[i]);
                stateList[i] = false;
            }
        }

    public:
        RelayModule(const unsigned short numberOfModules, const unsigned short* modulePins, const bool* turnedOffStates):
            modulePins(modulePins),
            turnedOffStates(turnedOffStates),
            numberOfModules(numberOfModules),
            stateList(new bool[numberOfModules]) {
                initialize();
            }

        void set(const unsigned short& module, const bool& newState) {
            if (module >= numberOfModules || module < 0) return;
            if (stateList[module] == newState) return;
            
            digitalWrite(modulePins[module], newState == ON ? !turnedOffStates[module] : turnedOffStates[module]);

            stateList[module] = newState;
        }

        bool get(const unsigned short& module) {
            return stateList[module];
        }

};
