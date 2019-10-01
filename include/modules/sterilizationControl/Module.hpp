#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class SterilizationControlModule: public TimeScopedDeviceDriver {
    public:
        SterilizationControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Sterylizacja", -1, relayModule, sterilization,
                TimeScopedDeviceDriverSettings(CONTINUOUS_ON)) {}
};
