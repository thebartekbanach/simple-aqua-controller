#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/relayModule/Devices.hpp"

#include "../serviceMode/Events.hpp"

#include "Settings.hpp"
#include "Events.hpp"

#include "FeedingActionCreator.hpp"

class FeedingControlModule: public CommonSystemModuleWithSettings<FeedingControlModuleSettings> {
    private:
        RelayModule* relayModule;

        bool feedingActive = false;

        void startFeedingModeEvent() {
            feedingActive = true;

            actionManager->acquire(new FeedingActionCreator(
                rtc->GetDateTime(),
                settings.data().feedingLength * 60,
                eventBus
            ));

            relayModule->set(mainPump, OFF);
        }

    public:
        FeedingControlModule(RelayModule* relayModule):
            relayModule(relayModule),
            CommonSystemModuleWithSettings(FeedingControlModuleSettings()) {
                relayModule->set(mainPump, ON);
            }

        ushort getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            prompt* feedingPrompt = new prompt("Tryb karmienia",
                ActionReceiver<FeedingControlModule>::forCb(this, &FeedingControlModule::startFeedingModeEvent),
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
                    &SystemModuleSettings<FeedingControlModuleSettings>::saveSettings),
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

        void update(const RtcDateTime& time) {
            if (serviceModeActive) return;

            relayModule->set(mainPump, !feedingActive);
        }

        void onEvent(const int &moduleId, const int &eventCode, void* data = nullptr) {
            if (moduleId == FEEDING_MODULE_ID && eventCode == ABORT_FEEDING) {
                feedingActive = false;
            }

            if (moduleId == SERVICE_MODE_MODULE_ID) {
                serviceModeActive = eventCode;
            }
        }
};