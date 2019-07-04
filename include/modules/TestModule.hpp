#pragma once
#include "../system/SystemModule.hpp"
#include "../system/ActionReceiver.hpp"

#include "../utils/log.hpp"


class TestModule: public CommonSystemModule {
    private:
        menuNode* feedingSettings = new menuNode("Ustawienia karmienia", 0, nullptr);
        prompt* feedingAction;
        prompt** actions;
        menuNode** settings;
        ushort lastSecond = 0;

        void startFeedingAction() {
            logln("Feeding started!");
        }

    public:
        TestModule() {
            feedingAction = new prompt("Karmienie", receiverFor(this, &TestModule::startFeedingAction), enterEvent);

            actions = new prompt*[1] {
                feedingAction
            };

            settings = new menuNode*[1] {
                feedingSettings
            };
        }

        virtual int getSettingsSize() { logln("Returning size of settings"); return 0; }
        virtual void resetSettings() { }

        virtual MenuItemsResult<prompt> getActionsMenu() {
            return MenuItemsResult<prompt>(actions, 1);
        }

        virtual MenuItemsResult<menuNode> getSettingsMenu() {
            return MenuItemsResult<menuNode>(settings, 1);
        }

        virtual void update(const RtcDateTime &time) {
            ushort second = time.Second();

            if (lastSecond != second) {
                lastSecond = second;
                logln(time.Second());
            }
        }
        virtual void onEvent(const int &moduleId, const int &eventCode) { };
};
