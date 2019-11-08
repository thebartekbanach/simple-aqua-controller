#pragma once

#include "WaterLevelSensor.hpp"

class WaterLevelSensorDataStream {
    private:
        const unsigned short dataSetSize;
        unsigned short actualIndex = 0;
        bool* data;

    public:
        WaterLevelSensorDataStream(const unsigned short& dataSetSize, bool defaultState = false):
            dataSetSize(dataSetSize),
            data(new bool[dataSetSize]) {
                for (unsigned short i = 0; i < dataSetSize; ++i) {
                    data[i] = defaultState;
                }
            }
        
        ~WaterLevelSensorDataStream() {
            delete[] data;
        }

        float getAverage() {
            unsigned short successfullChecks = 0;

            for (unsigned short i = 0; i < dataSetSize; ++i) {
                successfullChecks += data[i];
            }

            if (successfullChecks == 0) return 0;
            else return (float)successfullChecks / dataSetSize;
        }

        float getUnsuccessfullAverage() {
            return 1 - getAverage();
        }

        float senseAndGetAverage(WaterLevelSensor* sensor, const unsigned short& deviceId, const unsigned short& level) {
            bool result = sensor->sense(deviceId, level);

            data[actualIndex] = result;
            
            if (actualIndex + 1 >= dataSetSize) {
                actualIndex = 0;
            } else {
                actualIndex += 1;
            }

            return getAverage();
        }

        float senseAndGetUnsuccessfullAverage(WaterLevelSensor* sensor, const unsigned short& deviceId, const unsigned short& level) {
            return 1 - senseAndGetAverage(sensor, deviceId, level);
        }
};
