#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class HeaterControlModule: public TimeScopedDeviceDriver {
    public:
        HeaterControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Ogrzewanie", -1, relayModule, heater,
                TimeScopedDeviceDriverSettings(CONTINUOUS_ON)) {}
};
