#pragma once

#include <Arduino.h>
#include <Servo.h>

#include "../../system/PreciseTimer.hpp"

#include "ValveModuleBase.hpp"

class ValveModuleUsingInternalPwm: public ValveModuleBase {
    private:
        Servo* servoValves;
        unsigned short* valveSteeringPins;
        unsigned short servosOpenAngle;
        unsigned short servosCloseAngle;

    private:
        void moveServo(unsigned short valveId, bool state) {
            if (!servoValves[valveId].attached()) {
                servoValves[valveId].attach(valveSteeringPins[valveId]);
            }

            servoValves[valveId].write(state ? servosOpenAngle : servosCloseAngle);
        }

        void detach(unsigned short valveId) {
            if (servoValves[valveId].attached()) {
                servoValves[valveId].detach();
            }
        }

    public:
        ValveModuleUsingInternalPwm(
            unsigned short numberOfValves,
            unsigned short servosOpenAngle,
            unsigned short servosCloseAngle,
            unsigned short remotesDetectionPin,
            unsigned short* valveSteeringPins,
            unsigned short* valveClosedDetectionPins):
                valveSteeringPins(valveSteeringPins),
                servosOpenAngle(servosOpenAngle),
                servosCloseAngle(servosCloseAngle),
                servoValves(new Servo[numberOfValves] { Servo() }),
                ValveModuleBase(
                    numberOfValves,
                    remotesDetectionPin,
                    closeDetectionPins
                ) {
                    initialize();
                }
};
