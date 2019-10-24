#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

#include "Events.hpp"

class HeaterControlModule: public TimeScopedDeviceDriver {
    private:
        bool heaterSafetyLock = false;

    public:
        HeaterControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Ogrzewanie", -1, relayModule, heater,
                TimeScopedDeviceDriverSettings(CONTINUOUS_ON)) {}

        bool isDeviceEnabled() override {
            return !heaterSafetyLock;
        }

        void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) override {
            if (moduleId == HEATER_MODULE_ID) {
                switch (eventCode) {
                    case HEATER_SAFETY_LOCK:
                        relayModule->set(heater, OFF);
                        heaterSafetyLock = true;
                        break;
                    case HEATER_SAFETY_UNLOCK:
                        heaterSafetyLock = false;
                        break;
                    default: break;
                }
            }
            
            TimeScopedDeviceDriver::onEvent(moduleId, eventCode, data);
        }
};
