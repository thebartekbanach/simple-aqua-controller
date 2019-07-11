#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/Devices.hpp"

#include "Settings.hpp"
#include "Events.hpp"

#include "FeedingActionCreator.hpp"

class MainPumpModule: public CommonSystemModuleWithSettings<MainPumpModuleSettings> {
    private:
        RelayModule* relayModule;

        void startFeedingModeEvent() {
            actionManager->acquire(new FeedingActionCreator(
                rtc->GetDateTime(),
                settings.data().feedingLength * 60,
                eventBus
            ));

            relayModule->set(mainPump, OFF);
        }

    public:
        MainPumpModule(RelayModule* relayModule):
            relayModule(relayModule),
            CommonSystemModuleWithSettings(MainPumpModuleSettings()) {
                relayModule->set(mainPump, ON);
            }

        ushort getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            prompt* feedingPrompt = new prompt("Tryb karmienia",
                ActionReceiver<MainPumpModule>::forCb(this, &MainPumpModule::startFeedingModeEvent),
                enterEvent);

            return new prompt*[1] {
                feedingPrompt
            };
        }

        ushort getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            prompt* feedingLength = new menuField<ushort>(
                settings.data().feedingLength,
                "Czas karmienia", "m", 5, 60, 1, 0,
                ActionReceiver<typeof(settings)>::forCb(&settings, 
                    &SystemModuleSettings<MainPumpModuleSettings>::saveSettings),
                exitEvent
            );

            prompt** feedingSettingsMenuItems = new prompt*[1] {
                feedingLength
            };

            menuNode* feedingSettingsMenu = new menuNode("Karmienie", 1, feedingSettingsMenuItems);

            menuNode** result = new menuNode*[1] {
                feedingSettingsMenu
            };

            return result;
        }

        void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) {
            if (moduleId == MAIN_PUMP_FEEDING_ACTION && eventCode == ABORT_FEEDING) {
                relayModule->set(mainPump, ON);
            }
        }
};