#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"

#include "../../control/valves/ValveModule.hpp"

#include "../../menu/YesNoToggle.hpp"

#include "Settings.hpp"

#include "actionCreators/RemoveWater.hpp"
#include "actionCreators/ChangeWaterManually.hpp"

#include "../../control/valves/actionCreators/ConnectExternalWaterControl.hpp"

class WaterChangeModule: public CommonSystemModuleWithSettings<WaterChangeModuleSettings> {
    private:
        RelayModule* relayModule;
        WaterLevelSensor* waterLevelSensor;
        ValveModule* valveModule;

        void automaticallyChangeWater() {
            actionManager->acquire(
                new ConnectExternalWaterControl(
                    valveModule, "   Podmiana wody",
                    new RemoveWaterActionCreator(
                        relayModule,
                        waterLevelSensor,
                        valveModule,
                        eventBus,
                        &settings.data(),
                        rtc->GetDateTime()
            )));
        }

        void manuallyChangeWater() {
            actionManager->acquire(new ConnectExternalWaterControl(
                valveModule, "   Podmiana wody",
                new ChangeWaterManuallyActionCreator(
                    relayModule,
                    valveModule,
                    eventBus
                )
            ));
        }

        void saveSettings() {
            settings.saveSettings();
        }

    public:
        WaterChangeModule(
            RelayModule* relayModule,
            WaterLevelSensor* waterLevelSensor,
            ValveModule* valveModule):
                relayModule(relayModule),
                waterLevelSensor(waterLevelSensor),
                valveModule(valveModule),
                CommonSystemModuleWithSettings<WaterChangeModuleSettings>(
                    WaterChangeModuleSettings()
                ) {}

        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto saveSettingsActionReceiver = new ActionReceiver<WaterChangeModule>(
                this, &WaterChangeModule::saveSettings
            );

            prompt** addionalWaterTankRefillSettings = new prompt*[1] {
                YesNoToggle("Odblokowane: ",
                    settings.data().refillAddionalWaterTankDuringWaterChange,
                    saveSettingsActionReceiver)
            };

            prompt** timeoutSettings = new prompt*[4] {
                new menuField<float>(settings.data().aquariumRefillTimeout,
                    "Czas wlewu", "m", 0.5, 90, 1, 0.5, saveSettingsActionReceiver, exitEvent),
                
                new menuField<float>(settings.data().aquariumWaterOutflowTimeout,
                    "Czas wylewu", "m", 0.5, 90, 1, 0.5, saveSettingsActionReceiver, exitEvent),

                new menuField<float>(settings.data().addionalWaterTankRefillTimeout,
                    "Uzup. dolewki", "m", 0.5, 90, 1, 0.5, saveSettingsActionReceiver, exitEvent),
                    
                new menuField<float>(settings.data().aquariumRefillTimeout,
                    "Uzup. braku", "m", 0.5, 90, 1, 0.5, saveSettingsActionReceiver, exitEvent),
            };

            prompt** settingItems = new prompt*[2] {
                new menuNode("Auto uzup. rezerwy", 1, addionalWaterTankRefillSettings),
                new menuNode("Bezpieczenstwo", 3, timeoutSettings)
            };

            return new menuNode*[1] {
                new menuNode("Podmiana wody", 2, settingItems)
            };
        }

        unsigned short getActionMenuItemsLength() { return 1; }

        prompt** getActionMenuItems() {
            auto autoWaterChangeActionReceiver = new ActionReceiver<WaterChangeModule>(
                this, &WaterChangeModule::automaticallyChangeWater
            );

            auto manualWaterChangeActionReceiver = new ActionReceiver<WaterChangeModule>(
                this, &WaterChangeModule::manuallyChangeWater
            );

            prompt** waterChangeActions = new prompt*[2] {
                new prompt("Auto podmiana", autoWaterChangeActionReceiver, enterEvent),
                new prompt("Reczna podmiana", manualWaterChangeActionReceiver, enterEvent)
            };

            return new prompt*[1] {
                new menuNode("Podmiana wody", 2, waterChangeActions)
            };
        }
};
