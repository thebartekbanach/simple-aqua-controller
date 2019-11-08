#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/PreciseTimer.hpp"

#include "../../menu/YesNoToggle.hpp"

#include "ValveModule.hpp"
#include "Settings.hpp"

class ValveModuleBase: public ValveModule, public CommonSystemModuleWithSettings<ValveModuleBaseSettings>{
    private:
        unsigned long* lastMoveTime;
        bool* attached;

        void move(unsigned short valveId, bool state) {
            lastMoveTime[valveId] = millis();
            attached[valveId] = true;

            moveServo(valveId, state);
        }

        void saveSettings() {
            settings.saveSettings();
        }

    protected:
        unsigned short numberOfValves;
        unsigned short remotesDetectionPin;
        unsigned short* closeDetectionPins;

        ValveModuleBase(
            unsigned short numberOfValves,
            unsigned short remotesDetectionPin,
            unsigned short* closeDetectionPins
        ):
            numberOfValves(numberOfValves),
            remotesDetectionPin(remotesDetectionPin),
            closeDetectionPins(closeDetectionPins),
            lastMoveTime(new unsigned long[numberOfValves]),
            attached(new bool[numberOfValves]),
            CommonSystemModuleWithSettings<ValveModuleBaseSettings>(
                ValveModuleBaseSettings()
            ) {}

        void initialize() {
            pinMode(remotesDetectionPin, INPUT_PULLUP);

            for (unsigned short i = 0; i < numberOfValves; ++i) {
                pinMode(closeDetectionPins[i], INPUT_PULLUP);
                move(i, false);
            }
        }

        virtual void moveServo(unsigned short valveId, bool state) = 0;
        virtual void detach(unsigned short valveId) = 0;

    public:
        #pragma region SETTINGS_MENU_DEFINITION
        
        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
            auto saveSettings = new ActionReceiver<ValveModuleBase>(this, &ValveModuleBase::saveSettings);

            prompt** settings = new prompt*[3] {
                new menuField<unsigned short>(
                    this->settings.data().maxTimeToMove,
                    "Czas ruchu: ", "ms",
                    1000, 3000, 100, 10,
                    saveSettings, exitEvent
                ),

                YesNoToggle(
                    "Auto detach: ",
                    this->settings.data().autoDetachEnabled,
                    saveSettings
                ),
                
                new menuField<unsigned short>(
                    this->settings.data().autoDetachTime,
                    "Detach po: ", "ms",
                    1000, 50000, 100, 10,
                    saveSettings, exitEvent
                )
            };
            
            return new menuNode*[1] {
                new menuNode("Serwozawory", 3, settings)
            };
        }


        #pragma endregion

        #pragma region DRIVER_FUNCTIONS

        bool open(unsigned short valveId) {
            return set(valveId, true);
        }

        bool close(unsigned short valveId) {
            return set(valveId, false);
        }

        virtual bool set(unsigned short valveId, bool state) {
            if (valveId >= numberOfValves) return false;
            if (isClosed(valveId) != state) return true;

            PreciseTimer moveTimer(settings.data().maxTimeToMove);

            move(valveId, state);

            while (!moveTimer.done()) {
                if (state != isClosed(valveId)) {
                    return true;
                }
            }

            return false;
        }

        virtual bool isClosed(unsigned short valveId) {
            if (valveId >= numberOfValves) return false;
            return !digitalRead(closeDetectionPins[valveId]);
        }

        virtual bool areRemotesAvailable() {
            return !digitalRead(remotesDetectionPin);
        }
        
        virtual void update(const RtcDateTime &time) {
            if (!settings.data().autoDetachEnabled) return;

            PreciseTimer detachCheckTimer;

            for (unsigned short i = 0; i < numberOfValves; ++i) {
                if (!attached[i]) continue;

                detachCheckTimer.start((unsigned long)settings.data().autoDetachTime, lastMoveTime[i]);

                if (detachCheckTimer.done()) {
                    detach(i);
                    lastMoveTime[i] = 0;
                    attached[i] = 0;
                }
            }
        }
        
        #pragma endregion
};