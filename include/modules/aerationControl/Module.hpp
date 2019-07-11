#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class AerationControlModule: public TimeScopedDeviceDriver {
    public:
        AerationControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Napowietrzanie", -1, relayModule, aeration,
                TimeScopedDeviceDriverSettings(IN_NIGHT_CYCLE)
            ){}
};
