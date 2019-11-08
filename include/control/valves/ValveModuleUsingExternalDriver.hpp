#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "../../system/PreciseTimer.hpp"

#include "ValveModuleBase.hpp"

class ValveModuleUsingExternalDriver: public ValveModuleBase {
    private:
        Adafruit_PWMServoDriver* pwm;
        unsigned short servoMax;
        unsigned short servoMin;
        unsigned short servosOpenAngle;
        unsigned short servosCloseAngle;

    protected:
        void moveServo(unsigned short valveId, bool state) {
            pwm->setPWM(valveId, 0, map(
                state
                    ? servosOpenAngle
                    : servosCloseAngle,
                0, 180,
                servoMin,
                servoMax
            ));
        }
        
        void detach(unsigned short valveId) {
            pwm->setPWM(valveId, 0, 4096);
        }

    public:
        ValveModuleUsingExternalDriver(
            unsigned short i2cAddress,
            TwoWire& driverConnection,
            unsigned short pwmFrequency,
            unsigned short servoMax,
            unsigned short servoMin,
            unsigned short numberOfValves,
            unsigned short servosOpenAngle,
            unsigned short servosCloseAngle,
            unsigned short remotesDetectionPin,
            unsigned short* valveClosedDetectionPins):
                servoMax(servoMax),
                servoMin(servoMin),
                servosOpenAngle(servosOpenAngle),
                servosCloseAngle(servosCloseAngle),
                ValveModuleBase(
                    numberOfValves,
                    remotesDetectionPin,
                    valveClosedDetectionPins
                ) {
                    pwm = new Adafruit_PWMServoDriver(i2cAddress, driverConnection);

                    pwm->begin();
                    pwm->setPWMFreq(pwmFrequency);

                    initialize();
                }
};
