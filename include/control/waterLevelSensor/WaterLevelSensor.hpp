#pragma once

#include <Arduino.h>

#include "../../system/PreciseTimer.hpp"

#include "devices.hpp"

class WaterLevelSensor {
    private:
        const short unsigned int numberOfDevices;
        const short unsigned int* numberOfDevicePins;
        short unsigned int** devicesPins;

        void allToGnd() {
            for (unsigned short device = 0; device < numberOfDevices; ++device) {
                for (unsigned short pin = 0; pin < numberOfDevicePins[device]; ++pin) {
                    pinMode(devicesPins[device][pin], OUTPUT);
                    digitalWrite(devicesPins[device][pin], LOW);
                }
            }
        }

    public:
        WaterLevelSensor(const short unsigned int numberOfDevices, const short unsigned int* numberOfDevicePins, short unsigned int** devicesPins):
            numberOfDevices(numberOfDevices),
            numberOfDevicePins(numberOfDevicePins),
            devicesPins(devicesPins) {
                allToGnd();
            }

        bool sense(const unsigned short& deviceId, const unsigned short& level) {
            if (deviceId > numberOfDevices) return false;
            if (level > numberOfDevicePins[deviceId]) return false;
            
            pinMode(devicesPins[deviceId][level], INPUT_PULLUP);

            PreciseTimer senseTimer;
            unsigned short measurements = 0;

            for (unsigned short i = 0; i < 10;) {
                if (senseTimer.done()) { // average the result, it all takes ~100ms to check
                    measurements += !digitalRead(devicesPins[deviceId][level]);
                    senseTimer.start(10);
                    ++i;

                    if (measurements > 5) break;
                }
            }

            const bool result = measurements > 5;

            pinMode(devicesPins[deviceId][level], OUTPUT);
            digitalWrite(devicesPins[deviceId][level], LOW);

            return result;
        }
};
