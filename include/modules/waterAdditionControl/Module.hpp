#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/waterLevelSensor/WaterLevelSensor.hpp"

#include "../../menu/TurnStateToggle.hpp"
#include "../../menu/TimeField.hpp"

#include "Settings.hpp"

#include "actionCreators/WaterLevelChecking.hpp"

class WaterAdditionControlModule: public CommonSystemModuleWithSettings<WaterAdditionModuleSettings> {
    private:
        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;

        void settingsChanged() {
            settings.saveSettings();
        }

        void testWaterLevel() {
            if (actionManager->canAcquire()) {
                actionManager->acquire(new WaterLevelCheckingActionCreator(
                    settings.data().numberOfChecks,
                    settings.data().checkingFrequency,
                    settings.data().minNumberOfUnsuccessfullAttemps,
                    relayModule,
                    waterLevelSensor
                ));
            }
        }

        menuNode* getWorkModeSubmenu() {
            auto actionReceiver =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            prompt** items = new prompt*[3] {
                turnStateToggle("Tryb: ", settings.data().mode, actionReceiver),
                timeField("Sprawdza od", settings.data().canWorkFrom, actionReceiver),
                timeField("Sprawdza do", settings.data().canWorkTo, actionReceiver)
            };

            return new menuNode("Czasy pracy", 3, items);
        }

        menuNode* getFrequencySubmenu() {
            auto actionReceiver =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::settingsChanged);

            auto breaksBeetweenChecks = new menuField<ushort>(settings.data().breaksBetweenChecks,
                "Przerwy", "m", 5, 1000, 1, 0, actionReceiver, exitEvent);

            auto numberOfChecks = new menuField<ushort>(settings.data().numberOfChecks,
                "Ilosc probek", "", 1, 100, 1, 0, actionReceiver, exitEvent);

            auto checkingFrequency = new menuField<ushort>(settings.data().checkingFrequency,
                "Czestot.", "ms", 100, 5000, 10, 0, actionReceiver, exitEvent);

            auto percentOfUnsuccessfullAttemps = new menuField<ushort>(settings.data().minNumberOfUnsuccessfullAttemps,
                "Nieudane", "%", 1, 100, 1, 0, actionReceiver, exitEvent);

            prompt** items = new prompt*[4] {
                breaksBeetweenChecks,
                numberOfChecks,
                checkingFrequency,
                percentOfUnsuccessfullAttemps
            };

            return new menuNode("Probkowanie", 4, items);
        }
    
    public:
        WaterAdditionControlModule(RelayModule* relayModule, WaterLevelSensor* waterLevelSensor):
            relayModule(relayModule),
            waterLevelSensor(waterLevelSensor),
            CommonSystemModuleWithSettings<WaterAdditionModuleSettings>(
                WaterAdditionModuleSettings()
            ){}

        ushort getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            prompt** submenus = new prompt*[2] {
                getWorkModeSubmenu(),
                getFrequencySubmenu()
            };

            menuNode* settingsMenu = new menuNode("Dolewka wody", 2, submenus);

            return new menuNode*[1] {
                settingsMenu
            };
        }

        ushort getActionMenuItemsLength() { return 2; }

        prompt** getActionMenuItems() {
            auto testWaterLevelActionCreator =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::testWaterLevel);

            auto fillWaterTank =
                new ActionReceiver<WaterAdditionControlModule>(this, &WaterAdditionControlModule::testWaterLevel);

            return new prompt*[2] {
                new prompt("Dolej wody", testWaterLevelActionCreator, enterEvent),
                new prompt("Uzupelnij rezerwe", fillWaterTank, enterEvent)
            };
        }
};
