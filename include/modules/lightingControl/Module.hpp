#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class LightingControlModule: public TimeScopedDeviceDriver {
    public:
        LightingControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Oswietlenie", -1, relayModule, lighting,
                TimeScopedDeviceDriverSettings(IN_DAY_CYCLE)) {}
};
