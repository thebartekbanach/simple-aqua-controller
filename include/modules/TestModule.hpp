#pragma once
#include "../system/SystemModule.hpp"
#include "../system/ActionReceiver.hpp"
#include "../system/Timer.hpp"

#include "../utils/log.hpp"

class TestAction: public CommonActionCreator {
    private:
        Timer actionEnd;
        int lastSecond = 0;

    protected:
        void setup() override {
            logln("TestAction setup called");
            lcd->clear();
            lcd->setCursor(0, 0);
            lcd->print("Czas karmienia");
            lcd->setCursor(0, 1);
            lcd->print("dobiegnie konca");
            lcd->setCursor(0, 2);
            lcd->print("za: ");
        }

    public:
        TestAction(const Timer &actionEndTime) {
            actionEnd = actionEndTime;
        }

        ActionCreator* update(const RtcDateTime &time, const JoystickActions &action) {
            const int timeToEnd = actionEnd.timeToEndInSeconds(time);

            if (lastSecond != timeToEnd) {
                lcd->setCursor(5, 2);
                lcd->print((String(timeToEnd) + "  ").c_str());
                lastSecond = timeToEnd;
            } else if (timeToEnd < 1) {
                return nullptr;
            }
            
            return this;
        }
};

class TestModule: public CommonSystemModule {
    private:
        menuNode* feedingSettings = new menuNode("Ustawienia karmienia", 0, nullptr);
        prompt* feedingAction;
        prompt** actions;
        menuNode** settings;
        ushort lastSecond = 0;

        void startFeedingAction() {
            logln("Feeding started!");
            actionManager->acquire(new TestAction(Timer().start(rtc->GetDateTime(), RtcDateTime(30))));
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
