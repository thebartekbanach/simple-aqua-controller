#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/Timer.hpp"

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "../../menu/TurnStateToggle.hpp"
#include "../../menu/TimeField.hpp"

#include "../../utils/isInTimeScope.hpp"

#include "../timeSetup/DayCycle.hpp"

#include "Events.hpp"
#include "Settings.hpp"

#include "actionCreators/WaterLevelChecking.hpp"
#include "actionCreators/FillAddionalWaterTank.hpp"

#include "actionCreators/messages/ConnectCleanWaterSource.hpp"

class WaterAdditionControlModule: public CommonSystemModuleWithSettings<WaterAdditionModuleSettings> {
    private:
        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;

        Timer waterAdditionTimer;
        DayCycle actualDayCycle;

        bool checkingEnabled = false;

        void settingsChanged() {
            settings.saveSettings();
        }

        void testWaterLevel() {
            if (!actionManager->isUsedByCreator()) {
                actionManager->acquire(new WaterLevelCheckingActionCreator(
                    settings.data(),
                    relayModule,
                    waterLevelSensor
                ));
            }
        }

        void startAddionalWaterTankRefill() {
            if (!actionManager->isUsedByCreator()) {
                actionManager->acquire(connectCleanWaterSourceMessage(
                    new FillAddionalWaterTankActionCreator(
                        settings.data().addionalWaterTankRefillTimeout,
                        waterLevelSensor,
                        relayModule
                    )
                ));
            } 
        }

        menuNode* getWorkModeSubmenu() {
            auto saveSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            auto workingModeValues = new prompt*[4] {
                new menuValue<WAM_WorkingMode>("wylaczony", DISABLED),
                new menuValue<WAM_WorkingMode>("manualny", MANUAL),
                new menuValue<WAM_WorkingMode>("dzienny", DAY_CYCLE),
                new menuValue<WAM_WorkingMode>("nocny", NIGHT_CYCLE),
            };

            auto workingModeToggle = new toggle<WAM_WorkingMode>(
                "Tryb: ", settings.data().workingMode, 4, workingModeValues, saveSettings);


            auto manualWorkingModeSettingsItems = new prompt*[2] {
                timeField("Czas wlaczenia", settings.data().manualWorkStart, saveSettings),
                timeField("Czas wylaczenia", settings.data().manualWorkEnd, saveSettings)
            };

            auto manualWorkingModeMenu = new menuNode("Ustawienia manualne", 2, manualWorkingModeSettingsItems);


            auto timeShiftField = new menuField<ushort>(
                settings.data().workTimeShift, "Przesuniecie", "m", 0, 120, 1, 0, saveSettings
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

            auto waterAdditionTimeout = new menuField<ushort>(settings.data().waterAdditionTimeout,
                "Czas dolewki", "s", 5, 9999, 1, 0, saveSettings, exitEvent);

            auto waterRefillTimeout = new menuField<ushort>(settings.data().addionalWaterTankRefillTimeout,
                "Czas uzupel.", "s", 5, 9999, 1, 0, saveSettings, exitEvent);

            auto timeoutMenuItems = new prompt*[2] {
                waterAdditionTimeout,
                waterRefillTimeout
            };

            return new menuNode("Zabezpiecz. czasowe", 2, timeoutMenuItems);
        }

        menuNode* getFrequencySubmenu() {
            auto saveSettings =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            auto breaksBeetweenChecks = new menuField<ushort>(settings.data().breaksBetweenChecks,
                "Przerwy", "m", 5, 999, 1, 0, saveSettings, exitEvent);

            auto numberOfChecks = new menuField<ushort>(settings.data().numberOfChecks,
                "Ilosc probek", "", 1, 99, 1, 0, saveSettings, exitEvent);

            auto checkingFrequency = new menuField<ushort>(settings.data().checkingFrequency,
                "Czestotliwosc", "s", 1, 9, 1, 0, saveSettings, exitEvent);

            auto percentOfUnsuccessfullAttempts = new menuField<ushort>(settings.data().minNumberOfUnsuccessfullAttempts,
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
        WaterAdditionControlModule(RelayModule* relayModule, WaterLevelSensor* waterLevelSensor):
            relayModule(relayModule),
            waterLevelSensor(waterLevelSensor),
            CommonSystemModuleWithSettings<WaterAdditionModuleSettings>(
                WaterAdditionModuleSettings()
            ) {}

        ushort getSettingsMenuItemsLength() { return 1; }

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

        ushort getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            auto testWaterLevelActionCreator =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::testWaterLevel);

            auto fillWaterTank =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::startAddionalWaterTankRefill);

            prompt** waterAdditionActionsMenuItems = new prompt*[2] {
                new prompt("Dolej wody", testWaterLevelActionCreator, enterEvent),
                new prompt("Uzupelnij rezerwe", fillWaterTank, enterEvent)
            };

            return new prompt*[1] {
                new menuNode("Dolewka wody", 2, waterAdditionActionsMenuItems)
            };
        }

        void setup() {
            waterAdditionTimer.start(rtc->GetDateTime(), settings.data().workTimeShift * 60);
        }

        void update(const RtcDateTime &time) {
            if (settings.data().workingMode == DISABLED) {
                checkingEnabled = false;
            } else if (settings.data().workingMode == MANUAL) {
                checkingEnabled = isInTimeScope(
                    settings.data().manualWorkStart,
                    settings.data().manualWorkEnd,
                    time
                );
            } else if (settings.data().workingMode == DAY_CYCLE) {
                checkingEnabled = actualDayCycle == DAY;
            } else if (settings.data().workingMode == NIGHT_CYCLE) {
                checkingEnabled = actualDayCycle == NIGHT;
            }

            if (!checkingEnabled) {
                relayModule->set(addionalPump, OFF);
                return;
            }

            if (waterAdditionTimer.isReached(time)) {
                if (!actionManager->canAcquire()) {
                    waterAdditionTimer.start(time, 5 * 60);
                    return;
                }

                actionManager->acquire(new WaterLevelCheckingActionCreator(
                    settings.data(),
                    relayModule,
                    waterLevelSensor
                ));

                waterAdditionTimer.start(time, settings.data().breaksBetweenChecks * 60);
            }
        }

        
        void onEvent(const int &moduleId, const int &eventCode, void* data) {
            if (moduleId == TIME_SETUP_MODULE_ID && eventCode == DAY_CYCLE_BEGIN) {
                actualDayCycle = DAY;
                return;
            }

            if (moduleId == TIME_SETUP_MODULE_ID && eventCode == NIGHT_CYCLE_BEGIN) {
                actualDayCycle = NIGHT;
                return;
            }
        }
};
