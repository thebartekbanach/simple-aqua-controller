#pragma once

#include <Arduino.h>

#include "devices.hpp"

class WaterLevelSensor {
    private:
        const short unsigned int numberOfDevices;
        const short unsigned int* numberOfDevicePins;
        short unsigned int** devicesPins;

        void allToGnd() {
            for (ushort device = 0; device < numberOfDevices; ++device) {
                for (ushort pin = 0; pin < numberOfDevicePins[device]; ++pin) {
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

        bool sense(const ushort& deviceId, const ushort& level) {
            if (deviceId > numberOfDevices) return false;
            if (level > numberOfDevicePins[deviceId]) return false;
            
            pinMode(devicesPins[deviceId][level], INPUT_PULLUP);
            delay(100); // need to wait a short while to give time to pull up the pin
            const bool result = !digitalRead(devicesPins[deviceId][level]);
            pinMode(devicesPins[deviceId][level], OUTPUT);
            digitalWrite(devicesPins[deviceId][level], LOW);

            return result;
        }
};
