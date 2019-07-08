#pragma once
#include "../system/SystemModule.hpp"
#include "../system/ActionReceiver.hpp"
#include "../system/Timer.hpp"

#include "../utils/log.hpp"

struct TestSettings {
    int feedingTime = 10;
};

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

class TestModule: public CommonSystemModuleWithSettings<TestSettings> {
    private:
        ushort lastSecond = 0;

        void startFeedingAction() {
            logln("Feeding started!");
            actionManager->acquire(new TestAction(Timer().start(rtc->GetDateTime(), RtcDateTime(settings.data().feedingTime))));
            settings.data().feedingTime += 1;
            settings.saveSettings();
        }

    public:
        TestModule(): CommonSystemModuleWithSettings(TestSettings()) {}

        prompt** getActionMenuItems() {
            return new prompt*[1] {
                new prompt("Karmienie", ActionReceiver<TestModule>::forCb(this, &TestModule::startFeedingAction), enterEvent)
            };
        }

        ushort getActionMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            return new menuNode*[1] {
                new menuNode("Karmienie", 0, nullptr)
            };
        }

        ushort getSettingsMenuItemsLength() { return 1; }

        void update(const RtcDateTime &time) {
            ushort second = time.Second();

            if (lastSecond != second) {
                lastSecond = second;
                logln(time.Second());
            }
        }

        void onEvent(const int &moduleId, const int &eventCode) { };
};
