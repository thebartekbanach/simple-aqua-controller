#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "../../system/PreciseTimer.hpp"

#include "ValveModule.hpp"

class ValveModuleUsingExternalDriver: public ValveModule, public ValveModuleAttachManagement {
    private:
        Adafruit_PWMServoDriver* pwm;

        bool* attachedValves;

        unsigned short noOfValves;
        unsigned short* valveClosedDetectionPins;
        unsigned short servoMax;
        unsigned short servoMin;
        unsigned short servosOpenAngle;
        unsigned short servosCloseAngle;
        unsigned int waitTime;
        unsigned short remotesDetectionPin;

        void move(unsigned short valveId, unsigned short angle) {
            attachedValves[valveId] = true;
            pwm->setPWM(valveId, 0, map(
                angle,
                0, 180,
                servoMin,
                servoMax
            ));
        }

    public:
        ValveModuleUsingExternalDriver(
            unsigned short i2cAddress,
            TwoWire& driverConnection,
            unsigned short pwmFrequency,
            unsigned short servoMax,
            unsigned short servoMin,
            unsigned short noOfValves,
            unsigned short servosOpenAngle,
            unsigned short servosCloseAngle,
            unsigned int waitTime,
            unsigned short remotesDetectionPin,
            unsigned short* valveClosedDetectionPins):
                noOfValves(noOfValves),
                servoMax(servoMax),
                servoMin(servoMin),
                attachedValves(new bool[noOfValves]),
                valveClosedDetectionPins(valveClosedDetectionPins),
                servosOpenAngle(servosOpenAngle),
                servosCloseAngle(servosCloseAngle),
                waitTime(waitTime),
                remotesDetectionPin(remotesDetectionPin) {
                    pwm = new Adafruit_PWMServoDriver(i2cAddress, driverConnection);

                    pwm->begin();
                    pwm->setPWMFreq(pwmFrequency);

                    pinMode(remotesDetectionPin, INPUT_PULLUP);

                    for (unsigned short i = 0; i < noOfValves; ++i) {
                        pinMode(valveClosedDetectionPins[i], INPUT_PULLUP);
                        move(i, servosCloseAngle);
                        attachedValves[i] = true;
                    }
        }

        bool open(unsigned short valveId) {
            return set(valveId, true);
        }

        bool close(unsigned short valveId) {
            return set(valveId, false);
        }

        bool set(unsigned short valveId, bool state) {
            if (state != isClosed(valveId)) return true;
            
            move(valveId, state == true ? servosOpenAngle : servosCloseAngle);

            PreciseTimer servoMoveTimer(waitTime);

            while (!servoMoveTimer.done()) {
                const bool closed = isClosed(valveId);

                if (state == true && !closed) {
                    return true;
                }

                if (state == false && closed) {
                    delay(200);
                    detach(valveId);
                    return true;
                }
            }

            detach(valveId);
            return false;
        }

        bool isClosed(unsigned short valveId) {
            if (valveId >= noOfValves) return false;
            return !digitalRead(valveClosedDetectionPins[valveId]);
        }

        bool areRemotesAvailable() {
            return !digitalRead(remotesDetectionPin);
        }

        unsigned short getNumberOfValves() {
            return noOfValves;
        }

        bool attached(unsigned short valveId) {
            return attachedValves[valveId];
        }

        void detach(unsigned short valveId) {
            if (attached(valveId)) {
                pwm->setPWM(valveId, 0, 4096);
                attachedValves[valveId] = false;
            }
        }
};
