#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/Timer.hpp"

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../control/valves/ValveModule.hpp"

#include "../../control/valves/actionCreators/ConnectExternalWaterControl.hpp"

#include "../../menu/TurnStateToggle.hpp"
#include "../../menu/TimeField.hpp"

#include "../../utils/isInTimeScope.hpp"

#include "../timeSetup/DayCycle.hpp"
#include "../timeSetup/Events.hpp"

#include "../serviceMode/Events.hpp"

#include "Events.hpp"
#include "Settings.hpp"

#include "actionCreators/WaterLevelChecking.hpp"
#include "actionCreators/FillAddionalWaterTank.hpp"
#include "actionCreators/EmptyAddionalWaterTank.hpp"

#include "actionCreators/messages/AddionalWaterTankRefillUnnecessary.hpp"
#include "actionCreators/messages/AddionalWaterTankIsAlredyEmpty.hpp"

class WaterAdditionControlModule: public CommonSystemModuleWithSettings<WaterAdditionModuleSettings> {
    private:
        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;

        Timer waterAdditionTimer;
        DayCycle actualDayCycle;

        bool checkingEnabled = false;
        bool actionCreatorRc = false;

        void settingsChanged() {
            settings.saveSettings();
        }

        void workTimesChanged() {
            const RtcDateTime time = rtc->GetDateTime();

            if (isCheckingEnabled(time)) {
                waterAdditionTimer.start(time, settings.data().breaksBetweenChecks * 60);
            }

            settingsChanged();
        }

        void testWaterLevel() {
            if (!actionManager->isUsedByCreator()) {
                actionManager->acquire(
                    new WaterLevelCheckingActionCreator(
                        settings.data(),
                        relayModule,
                        waterLevelSensor,
                        eventBus
                ));
            }
        }

        void startAddionalWaterTankRefill() {
            if (!actionManager->isUsedByCreator()) {
                if (waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel)) {
                    actionManager->acquire(addionalWaterTankRefillUnnecessaryMessage());
                } else {
                    actionManager->acquire(new ConnectExternalWaterControl(
                        valveModule, "    Dolewka wody",
                        new FillAddionalWaterTankActionCreator(
                            settings.data().addionalWaterTankRefillTimeout,
                            waterLevelSensor,
                            valveModule,
                            eventBus
                    )));
                }
            } 
        }

        void startAddionalWaterTankEmptying() {
            if (!actionManager->isUsedByCreator()) {
                if (!waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel)) {
                    actionManager->acquire(addionalWaterTankIsAlredyEmpty());
                } else {
                    actionManager->acquire(new ConnectExternalWaterControl(
                        valveModule, "Oproznianie dolewki",
                        new EmptyAddionalWaterTankActionCreator(
                            settings.data().addionalWaterTankEmptyingTimeout,
                            relayModule,
                            waterLevelSensor,
                            valveModule,
                            eventBus
                        )
                    ));
                }
            }
        }

        menuNode* getWorkModeSubmenu() {
            auto saveSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::workTimesChanged);

            auto workingModeValues = new prompt*[5] {
                new menuValue<WAM_WorkingMode>("wylaczony", DISABLED),
                new menuValue<WAM_WorkingMode>("ciagly", CONTINUOUS),
                new menuValue<WAM_WorkingMode>("manualny", MANUAL),
                new menuValue<WAM_WorkingMode>("dzienny", DAY_CYCLE),
                new menuValue<WAM_WorkingMode>("nocny", NIGHT_CYCLE),
            };

            auto workingModeToggle = new toggle<WAM_WorkingMode>(
                "Tryb: ", settings.data().workingMode, 5, workingModeValues, saveSettings);


            auto manualWorkingModeSettingsItems = new prompt*[2] {
                timeField("Czas wlaczenia", settings.data().manualWorkStart, saveSettings),
                timeField("Czas wylaczenia", settings.data().manualWorkEnd, saveSettings)
            };

            auto manualWorkingModeMenu = new menuNode("Ustawienia zakresu", 2, manualWorkingModeSettingsItems);


            auto timeShiftField = new menuField<unsigned short>(
                settings.data().workTimeShift, "Przesuniecie", "m", 0, 120, 1, 0, saveSettings, exitEvent
            );


            auto menuItems = new prompt*[3] {
                workingModeToggle,
                timeShiftField,
                manualWorkingModeMenu
            };

            return new menuNode("Czasy pracy", 3, menuItems);
        }

        menuNode* getTimeoutMenu() {
            auto saveSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            auto waterAdditionTimeout = new menuField<float>(settings.data().waterAdditionTimeout,
                "Czas dolewki", "m", 0.5, 90, 1, 0.5, saveSettings, exitEvent);

            auto waterRefillTimeout = new menuField<float>(settings.data().addionalWaterTankRefillTimeout,
                "Czas uzupel.", "m", 0.5, 90, 1, 0.5, saveSettings, exitEvent);
                
            auto waterEmptyingTimeout = new menuField<float>(settings.data().addionalWaterTankRefillTimeout,
                "Czas oprozn.", "m", 0.5, 90, 1, 0.5, saveSettings, exitEvent);

            auto timeoutMenuItems = new prompt*[3] {
                waterAdditionTimeout,
                waterRefillTimeout,
                waterEmptyingTimeout
            };

            return new menuNode("Bezpieczenstwo", 3, timeoutMenuItems);
        }

        menuNode* getFrequencySubmenu() {
            auto saveSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            auto saveWorkTimeSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::workTimesChanged);

            auto breaksBeetweenChecks = new menuField<unsigned short>(settings.data().breaksBetweenChecks,
                "Przerwy", "m", 5, 24 * 60, 10, 1, saveWorkTimeSettings, exitEvent);

            auto numberOfChecks = new menuField<unsigned short>(settings.data().numberOfChecks,
                "Ilosc probek", "", 1, 20, 1, 0, saveSettings, exitEvent);

            auto checkingFrequency = new menuField<unsigned short>(settings.data().checkingFrequency,
                "Czestotliwosc", "ms", 100, 5000, 100, 50, saveSettings, exitEvent);

            auto percentOfUnsuccessfullAttempts = new menuField<unsigned short>(settings.data().minNumberOfUnsuccessfullAttempts,
                "Min nieudanych", "%", 1, 99, 1, 0, saveSettings, exitEvent);


            prompt** items = new prompt*[4] {
                breaksBeetweenChecks,
                numberOfChecks,
                checkingFrequency,
                percentOfUnsuccessfullAttempts
            };

            return new menuNode("Probkowanie", 4, items);
        }
    
    public:
        WaterAdditionControlModule(RelayModule* relayModule, WaterLevelSensor* waterLevelSensor, ValveModule* valveModule):
            relayModule(relayModule),
            waterLevelSensor(waterLevelSensor),
            valveModule(valveModule),
            CommonSystemModuleWithSettings<WaterAdditionModuleSettings>(
                WaterAdditionModuleSettings()
            ) {}

        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            prompt** submenus = new prompt*[3] {
                getWorkModeSubmenu(),
                getFrequencySubmenu(),
                getTimeoutMenu()
            };

            menuNode* settingsMenu = new menuNode("Dolewka wody", 3, submenus);

            return new menuNode*[1] {
                settingsMenu
            };
        }

        unsigned short getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            auto testWaterLevelActionCreator =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::testWaterLevel);

            auto fillWaterTank =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::startAddionalWaterTankRefill);

            auto emptyWaterTank =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::startAddionalWaterTankEmptying);

            prompt** waterAdditionActionsMenuItems = new prompt*[3] {
                new prompt("Dolej wody", testWaterLevelActionCreator, enterEvent),
                new prompt("Uzupelnij rezerwe", fillWaterTank, enterEvent),
                new prompt("Oproznij rezerwe", emptyWaterTank, enterEvent)
            };

            return new prompt*[1] {
                new menuNode("Dolewka wody", 3, waterAdditionActionsMenuItems)
            };
        }
        

        void setup() {
            waterAdditionTimer.start(rtc->GetDateTime(), settings.data().workTimeShift * 60);
        }

        bool isCheckingEnabled(const RtcDateTime &time) {
            switch (settings.data().workingMode) {
                case CONTINUOUS:
                    return true;

                case MANUAL:
                    return isInTimeScope(
                        settings.data().manualWorkStart,
                        settings.data().manualWorkEnd,
                        time
                    );

                case DAY_CYCLE:
                    return actualDayCycle == DAY;

                case NIGHT_CYCLE:
                    return actualDayCycle == NIGHT;

                default:
                case DISABLED:
                    return false;
            }
        }

        void update(const RtcDateTime &time) {
            if (actionCreatorRc || serviceModeActive) return;

            if (!isCheckingEnabled(time)) {
                relayModule->set(addionalPump, OFF);
                waterAdditionTimer.stop();
                return;
            }

            if (waterAdditionTimer.isReached(time)) {
                if (!actionManager->canAcquire()) {
                    waterAdditionTimer.start(time, 1 * 60);
                    return;
                }

                actionManager->acquire(new WaterLevelCheckingActionCreator(
                    settings.data(),
                    relayModule,
                    waterLevelSensor,
                    eventBus
                ));

                waterAdditionTimer.start(time, settings.data().breaksBetweenChecks * 60);
            }
        }

        
        void onEvent(const int &moduleId, const int &eventCode, void* data) {
            if (moduleId == TIME_SETUP_MODULE_ID && eventCode == DAY_CYCLE_BEGIN) {
                actualDayCycle = DAY;

                if (settings.data().workingMode == DAY_CYCLE) {
                    waterAdditionTimer.start(rtc->GetDateTime(), settings.data().workTimeShift * 60);
                }

                return;
            }

            if (moduleId == TIME_SETUP_MODULE_ID && eventCode == NIGHT_CYCLE_BEGIN) {
                actualDayCycle = NIGHT;

                if (settings.data().workingMode == NIGHT_CYCLE) {
                    waterAdditionTimer.start(rtc->GetDateTime(), settings.data().workTimeShift * 60);
                }

                return;
            }

            if (moduleId == WATER_ADDITION_CONTROL_MODULE_ID) {
                actionCreatorRc = eventCode == WATER_ADDITION_CONTROL_ACQUIRE;
                return;
            }

            if (moduleId == SERVICE_MODE_MODULE_ID) {
                serviceModeActive = eventCode;

                RtcDateTime time = rtc->GetDateTime();

                if (!serviceModeActive && waterAdditionTimer.isReached(time) && isCheckingEnabled(time)) {
                    waterAdditionTimer.start(time, settings.data().workTimeShift * 60);
                }
            }
        }
};
