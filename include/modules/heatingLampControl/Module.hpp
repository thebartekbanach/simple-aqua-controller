#pragma once

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/devices.hpp"

#include "../../utils/TimeScopedDeviceDriver/Module.hpp"

class HeatingLampControlModule: public TimeScopedDeviceDriver {
    public:
        HeatingLampControlModule(RelayModule* relayModule):
            TimeScopedDeviceDriver("Lampa grzewcza", -1, relayModule, heatingLamp,
                TimeScopedDeviceDriverSettings(IN_DAY_CYCLE)) {}
};
