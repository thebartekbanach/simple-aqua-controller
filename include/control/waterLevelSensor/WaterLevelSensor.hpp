#pragma once

#include <Arduino.h>

#include "devices.hpp"

class WaterLevelSensor {
    private:
        const ushort numberOfDevices;
        const ushort* numberOfDevicePins;
        const ushort** devicesPins;

        void allToGnd() {
            for (ushort device = 0; device < numberOfDevices; ++device) {
                for (ushort pin = 0; pin < numberOfDevicePins[device]; ++pin) {
                    pinMode(devicesPins[device][pin], OUTPUT);
                    digitalWrite(devicesPins[device][pin], LOW);
                }
            }
        }

    public:
        WaterLevelSensor(const ushort numberOfDevices, const ushort* numberOfDevicePins, const ushort** devicesPins):
            numberOfDevices(numberOfDevices),
            numberOfDevicePins(numberOfDevicePins),
            devicesPins(devicesPins) {
                allToGnd();
            }

        bool sense(const ushort& deviceId, const ushort& level) {
            if (deviceId > numberOfDevices) return false;
            if (level > numberOfDevicePins[deviceId]) return false;
            
            pinMode(devicesPins[deviceId][level], INPUT_PULLUP);
            const bool result = !digitalRead(devicesPins[deviceId][level]);
            pinMode(devicesPins[deviceId][level], OUTPUT);
            digitalWrite(devicesPins[deviceId][level], LOW);

            return result;
        }
};
