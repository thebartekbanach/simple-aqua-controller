#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class HeatherControlModule: public TimeScopedDeviceDriver {
    public:
        HeatherControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Dogrzewanie", -1, relayModule, heather,
                TimeScopedDeviceDriverSettings(CONTINUOUS_ON)
            ){}
};
