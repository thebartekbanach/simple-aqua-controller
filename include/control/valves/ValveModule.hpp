#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "devices.hpp"

class ValveModule {
    Servo* servoValves;
    unsigned short noOfValves;
    unsigned short* valveSteeringPins;
    unsigned short* valveClosedDetectionPins;
    unsigned short servosOpenAngle;
    unsigned short servosCloseAngle;
    unsigned int waitTime;
    unsigned short remotesDetectionPin;

    public:
        ValveModule(
            unsigned short noOfValves,
            unsigned short servosOpenAngle,
            unsigned short servosCloseAngle,
            unsigned int waitTime,
            unsigned short remotesDetectionPin,
            unsigned short* valveSteeringPins,
            unsigned short* valveClosedDetectionPins):
                valveSteeringPins(valveSteeringPins),
                valveClosedDetectionPins(valveClosedDetectionPins),
                noOfValves(noOfValves),
                servoValves(new Servo[noOfValves] { Servo() }),
                servosOpenAngle(servosOpenAngle),
                servosCloseAngle(servosCloseAngle),
                waitTime(waitTime),
                remotesDetectionPin(remotesDetectionPin)
        {
            pinMode(remotesDetectionPin, INPUT_PULLUP);
            
            for (unsigned short i = 0; i < noOfValves; ++i) {
                pinMode(valveClosedDetectionPins[i], INPUT_PULLUP);

                if (!isClosed(i)) {
                    close(i);
                }
            }
        }

        bool open(unsigned short valveId) {
            return set(valveId, true);
        }

        bool close(unsigned short valveId) {
            return set(valveId, false);
        }

        bool set(unsigned short valveId, bool state) {
            if (valveId > noOfValves) return false;
            if (state == !isClosed(valveId)) return true;

            Servo& valve = servoValves[valveId];

            if (!valve.attached()) {
                valve.attach(valveSteeringPins[valveId]);
            }

            valve.write(state == true ? servosOpenAngle : servosCloseAngle);

            unsigned long servoMoveStart = millis();

            while (millis() - servoMoveStart < waitTime) {
                if (isClosed(valveId) == !state) break;
            }

            if (state == true) {
                return !isClosed(valveId);
            }

            valve.detach();
            return isClosed(valveId);
        }

        bool isClosed(unsigned short valveId) {
            if (valveId >= noOfValves) return false;
            return !digitalRead(valveClosedDetectionPins[valveId]);
        }

        bool areRemotesAvailable() {
            return !digitalRead(remotesDetectionPin);
        }
};
