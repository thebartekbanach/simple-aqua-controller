#pragma once

#include "WaterLevelSensor.hpp"

class WaterLevelSensorDataStream {
    private:
        const ushort& dataSetSize;
        bool* data;
        ushort actualIndex = 0;

    public:
        WaterLevelSensorDataStream(const ushort& dataSetSize):
            dataSetSize(dataSetSize),
            data(new bool[dataSetSize] { 0 }) {}
        
        ~WaterLevelSensorDataStream() {
            delete[] data;
        }

        float getAverage() {
            ushort successfullChecks = 0;

            for (ushort i = 0; i < dataSetSize; ++i) {
                successfullChecks += data[i];
            }

            return (float)successfullChecks / dataSetSize;
        }

        float getUnsuccessfullAverage() {
            return 1 - getAverage();
        }

        float senseAndGetAverage(WaterLevelSensor* sensor, const ushort& deviceId, const ushort& level) {
            bool result = sensor->sense(deviceId, level);

            data[actualIndex] = result;
            
            if (actualIndex + 1 >= dataSetSize) {
                actualIndex = 0;
            } else {
                actualIndex += 1;
            }

            return getAverage();
        }

        float senseAndGetUnsuccessfullAverage(WaterLevelSensor* sensor, const ushort& deviceId, const ushort& level) {
            return 1 - senseAndGetAverage(sensor, deviceId, level);
        }
};
