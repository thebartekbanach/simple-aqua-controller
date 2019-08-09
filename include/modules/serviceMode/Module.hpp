#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/Timer.hpp"

#include "../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../control/relayModule/RelayModule.hpp"

#include "../../menu/OnOffToggle.hpp"
#include "../../menu/YesNoToggle.hpp"

#include "Events.hpp"

class ServiceModeModule: public CommonSystemModule {
    private:
        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;
        navRoot* nav;
        
        void menuInOut(eventMask event, navNode& nav, prompt &item) {
            serviceModeEntered = event == enterEvent;

            if (event == enterEvent) {
                serviceModeEntered = true;
                this->nav->timeOut = 0;

                eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_ACTIVATION);
            }

            if (event == exitEvent) {
                serviceModeEntered = false;

                mainPumpActivated = false;
                addionalPumpActivated = false;
                heatherActivated = false;
                aerationActivated = false;
                sterilizationActivated = false;
                lightingActivated = false;
                addionalTankValveActivated = false;
                aquariumValveActivated = false;

                this->nav->timeOut = 60;

                eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_DEACTIVATION);
            }
        }

        void sensorsInOut(eventMask event, navNode& nav, prompt &item) {
            sensorsSubmenuEntered = event == enterEvent;

            if (event == exitEvent) {
                normalWaterLevelDetected = false;
                changeWaterLevelDetected = false;
                addionalWaterTankMaxLevelDetected = false;
                addionalWaterTankMinLevelDetected = false;
            }
        }

        menuNode* getDevicesControlSubmenu() {
            prompt** devices = new prompt*[8] {
                OnOffToggle("Glowna pompa: ", mainPumpActivated, nullptr),
                OnOffToggle("Pompa dolewki: ", addionalPumpActivated, nullptr),
                OnOffToggle("Grzalka: ", heatherActivated, nullptr),
                OnOffToggle("Napowietrzanie: ", aerationActivated, nullptr),
                OnOffToggle("Sterylizacja: ", sterilizationActivated, nullptr),
                OnOffToggle("Oswietlenie: ", lightingActivated, nullptr),
                OnOffToggle("Zawor rezerwy: ", addionalTankValveActivated, nullptr),
                OnOffToggle("Zawor akwarium: ", aquariumValveActivated, nullptr),
            };

            return new menuNode("Urzadzenia", 8, devices);
        }

        menuNode* getSensorsCheckSubmenu() {
            prompt** sensors = new prompt*[4] {
                YesNoToggle("Akwa. normalny: ", normalWaterLevelDetected, nullptr),
                YesNoToggle("Akwa. podmiana: ", changeWaterLevelDetected, nullptr),
                YesNoToggle("Rezerwa max: ", addionalWaterTankMaxLevelDetected, nullptr),
                YesNoToggle("Rezerwa min: ", addionalWaterTankMinLevelDetected, nullptr),
            };

            for (short i = 0; i < 4; ++i) {
                sensors[i]->disable();
            }
            
            auto inOutEvent = new ExactActionReceiver<ServiceModeModule>(this, &ServiceModeModule::sensorsInOut);

            return new menuNode("Sensory", 4, sensors, inOutEvent, (eventMask)(enterEvent|exitEvent));
        }

        void updateDevices() {
            relayModule->set(mainPump, mainPumpActivated);
            relayModule->set(addionalPump, addionalPumpActivated);
            relayModule->set(heather, heatherActivated);
            relayModule->set(aeration, aerationActivated);
            relayModule->set(sterilization, sterilizationActivated);
            relayModule->set(lighting, lightingActivated);
            relayModule->set(addionalTankValve, addionalTankValveActivated);
            relayModule->set(aquariumValve, aquariumValveActivated);
        }

        bool serviceModeEntered = false;
        bool sensorsSubmenuEntered = false;

        bool normalWaterLevelDetected = false;
        bool changeWaterLevelDetected = false;
        bool addionalWaterTankMaxLevelDetected = false;
        bool addionalWaterTankMinLevelDetected = false;

        Timer sensorsUpdateTimer;

        bool mainPumpActivated = false;
        bool addionalPumpActivated = false;
        bool heatherActivated = false;
        bool aerationActivated = false;
        bool sterilizationActivated = false;
        bool lightingActivated = false;
        bool addionalTankValveActivated = false;
        bool aquariumValveActivated = false;

    public:
        ServiceModeModule(
            WaterLevelSensor* waterLevelSensor,
            RelayModule* relayModule,
            navRoot* nav):
                waterLevelSensor(waterLevelSensor),
                relayModule(relayModule),
                nav(nav) {}

        ushort getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto inOutEvent = new ExactActionReceiver<ServiceModeModule>(this, &ServiceModeModule::menuInOut);

            prompt** submenus = new prompt*[2] {
                getSensorsCheckSubmenu(),
                getDevicesControlSubmenu()
            };

            return new menuNode*[1] {
                new menuNode("Tryb serwisowy", 2, submenus, inOutEvent, (eventMask)(enterEvent|exitEvent))
            };
        }

        void update(const RtcDateTime& time) {
            if (!serviceModeEntered) return;

            updateDevices();
            
            if (!sensorsSubmenuEntered) return;

            if (sensorsUpdateTimer.isReached(time)) {
                sensorsUpdateTimer.start(time, 1);

                normalWaterLevelDetected = waterLevelSensor->sense(aquariumWater, normalWaterLevel);
                changeWaterLevelDetected = waterLevelSensor->sense(aquariumWater, changeWaterLevel);
                addionalWaterTankMaxLevelDetected = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel);
                addionalWaterTankMinLevelDetected = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel);
            }
        }
};