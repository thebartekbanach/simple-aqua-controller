#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../control/relayModule/RelayModule.hpp"

#include "Settings.hpp"

class MainPumpModule: public CommonSystemModuleWithSettings<MainPumpModuleSettings> {
    private:
        RelayModule* relayModule;

        void startFeedingModeEvent() {

        }

    public:
        MainPumpModule(RelayModule* relayModule):
            relayModule(relayModule),
            CommonSystemModuleWithSettings(MainPumpModuleSettings()) {}

        ushort getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            prompt* feedingPrompt = new prompt("Tryb karmienia",
                ActionReceiver<MainPumpModule>::forCb(this, &MainPumpModule::startFeedingModeEvent),
                enterEvent);

            return new prompt*[1] {
                feedingPrompt
            };
        }
};