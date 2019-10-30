#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/Timer.hpp"

#include "../../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../../control/relayModule/RelayModule.hpp"
#include "../../control/valves/ValveModule.hpp"

#include "../../menu/OnOffToggle.hpp"
#include "../../menu/YesNoToggle.hpp"

#include "Events.hpp"

class ServiceModeModule: public CommonSystemModule {
    private:
        WaterLevelSensor* waterLevelSensor;
        RelayModule* relayModule;
        ValveModule* valveModule;
        navRoot* nav;

        bool serviceModeEntered = false;
        bool sensorsSubmenuEntered = false;
        bool valvesSubmenuEntered = false;

        Timer sensorsUpdateTimer;

        bool normalWaterLevelDetected = false;
        bool changeWaterLevelDetected = false;
        bool addionalWaterTankMaxLevelDetected = false;
        bool addionalWaterTankMinLevelDetected = false;

        bool isAquariumWaterValveClosed = false;
        bool isAddionalWaterTankValveClosed = false;
        bool isCleanWaterValveClosed = false;
        bool isSewageWaterValveClosed = false;

        bool isAquariumWaterValveActivated = false;
        bool isAddionalWaterTankValveActivated = false;
        bool isCleanWaterValveActivated = false;
        bool isSewageWaterValveActivated = false;
        bool areRemotesAvailable = false;

        bool mainPumpActivated = false;
        bool addionalPumpActivated = false;
        bool aerationActivated = false;
        bool sterilizationActivated = false;
        bool lightingActivated = false;
        bool heatingLampActivated = false;
        bool heaterActivated = false;
        
        void menuInOut(eventMask event, navNode& nav, prompt &item) {
            if (event == enterEvent) {
                serviceModeEntered = true;
                this->nav->timeOut = 0;

                eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_ACTIVATION);
            }

            if (event == exitEvent) {
                serviceModeEntered = false;

                mainPumpActivated = false;
                addionalPumpActivated = false;
                aerationActivated = false;
                sterilizationActivated = false;
                lightingActivated = false;
                heatingLampActivated = false;
                heaterActivated = false;

                updateDevices();

                isAquariumWaterValveActivated = false;
                isAddionalWaterTankValveActivated = false;
                isCleanWaterValveActivated = false;
                isSewageWaterValveActivated = false;

                updateValvesState();

                this->nav->timeOut = 60;

                eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_DEACTIVATION);
            }
        }

        void sensorsInOut(eventMask event, navNode& nav, prompt &item) {
            if (event == enterEvent) {
                sensorsSubmenuEntered = true;
            }

            else if (event == exitEvent) {
                normalWaterLevelDetected = false;
                changeWaterLevelDetected = false;
                addionalWaterTankMaxLevelDetected = false;
                addionalWaterTankMinLevelDetected = false;
            }
        }

        void valvesInOut(eventMask event, navNode& nav, prompt &item) {
            if (event == enterEvent) {
                valvesSubmenuEntered = true;
            }

            else if (event == exitEvent) {
                valvesSubmenuEntered = false;

                isAquariumWaterValveClosed = false;
                isAddionalWaterTankValveClosed = false;
                isCleanWaterValveClosed = false;
                isSewageWaterValveClosed = false;
            }
        }

        menuNode* getValveControlSubmenu(char* name, bool& activationState, bool& isClosed) {
            prompt** items = new prompt*[2] {
                YesNoToggle("Zamkniety: ", isClosed, nullptr),
                OnOffToggle("Stan: ", activationState,
                    new ActionReceiver<ServiceModeModule>(this, &ServiceModeModule::updateValvesState), eventMask::enterEvent
                )
            };

            items[0]->disable();

            return new menuNode(name, 2, items);
        }

        menuNode* getValvesControlSubmenu() {
            prompt** items = new prompt*[5] {
                getValveControlSubmenu("Obieg akwarium", isAquariumWaterValveActivated, isAquariumWaterValveClosed),
                getValveControlSubmenu("Zbiornik dolewki", isAddionalWaterTankValveActivated, isAddionalWaterTankValveClosed),
                getValveControlSubmenu("Czysta woda", isCleanWaterValveActivated, isCleanWaterValveClosed),
                getValveControlSubmenu("Scieki", isSewageWaterValveActivated, isSewageWaterValveClosed),
                YesNoToggle("Zdalne zawory: ", areRemotesAvailable, nullptr)
            };

            items[4]->disable();
            
            auto inOutEvent = new ExactActionReceiver<ServiceModeModule>(this, &ServiceModeModule::valvesInOut);
            
            return new menuNode("Zawory", 5, items, inOutEvent, (eventMask)(enterEvent|exitEvent));
        }

        menuNode* getDevicesControlSubmenu() {
            prompt** devices = new prompt*[7] {
                OnOffToggle("Glowna pompa: ", mainPumpActivated, nullptr),
                OnOffToggle("Pompa dolewki: ", addionalPumpActivated, nullptr),
                OnOffToggle("Napowietrzanie: ", aerationActivated, nullptr),
                OnOffToggle("Sterylizacja: ", sterilizationActivated, nullptr),
                OnOffToggle("Oswietlenie: ", lightingActivated, nullptr),
                OnOffToggle("Lampa grzewcza: ", heatingLampActivated, nullptr),
                OnOffToggle("Ogrzewanie: ", heaterActivated, nullptr)
            };

            return new menuNode("Urzadzenia", 7, devices);
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
            relayModule->set(aeration, aerationActivated);
            relayModule->set(sterilization, sterilizationActivated);
            relayModule->set(lighting, lightingActivated);
            relayModule->set(heatingLamp, heatingLampActivated);
            relayModule->set(heater, heaterActivated);
        }

        void updateSensors(const RtcDateTime& time) {
            if (!sensorsSubmenuEntered) return;

            if (sensorsUpdateTimer.isReached(time)) {
                sensorsUpdateTimer.start(time, 1);

                normalWaterLevelDetected = waterLevelSensor->sense(aquariumWater, normalWaterLevel);
                changeWaterLevelDetected = waterLevelSensor->sense(aquariumWater, changeWaterLevel);
                addionalWaterTankMaxLevelDetected = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMaxLevel);
                addionalWaterTankMinLevelDetected = waterLevelSensor->sense(addionalWaterTank, addionalWaterTankMinLevel);
            }
        }

        void updateValvesInfo() {
            if (!valvesSubmenuEntered) return;
            isAquariumWaterValveClosed = valveModule->isClosed(aquariumWaterValve);
            isAddionalWaterTankValveClosed = valveModule->isClosed(addionalWaterTankValve);
            isCleanWaterValveClosed = valveModule->isClosed(cleanWaterValve);
            isSewageWaterValveClosed = valveModule->isClosed(sewageWaterValve);
            areRemotesAvailable = valveModule->areRemotesAvailable();
        }

        void updateValvesState() {
            valveModule->set(aquariumWaterValve, isAquariumWaterValveActivated);
            valveModule->set(addionalWaterTankValve, isAddionalWaterTankValveActivated);
            valveModule->set(cleanWaterValve, isCleanWaterValveActivated);
            valveModule->set(sewageWaterValve, isSewageWaterValveActivated);
        }

    public:
        ServiceModeModule(
            WaterLevelSensor* waterLevelSensor,
            RelayModule* relayModule,
            ValveModule* valveModule,
            navRoot* nav):
                waterLevelSensor(waterLevelSensor),
                relayModule(relayModule),
                valveModule(valveModule),
                nav(nav) {}

        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto inOutEvent = new ExactActionReceiver<ServiceModeModule>(this, &ServiceModeModule::menuInOut);

            prompt** submenus = new prompt*[3] {
                getSensorsCheckSubmenu(),
                getDevicesControlSubmenu(),
                getValvesControlSubmenu()
            };

            return new menuNode*[1] {
                new menuNode("Tryb serwisowy", 3, submenus, inOutEvent, (eventMask)(enterEvent|exitEvent))
            };
        }

        void update(const RtcDateTime& time) {
            if (!serviceModeEntered) return;

            updateDevices();
            updateSensors(time);
            updateValvesInfo();
        }
};