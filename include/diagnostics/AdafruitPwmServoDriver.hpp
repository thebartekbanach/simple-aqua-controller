#define ADAFRUIT_PWM_SERVO_DRIVER_TEST_ENABLED false

#if ADAFRUIT_PWM_SERVO_DRIVER_TEST_ENABLED == true
    #define DIAGNOSTICS_MODE

    #include <Arduino.h>
    #include <Wire.h>
    #include <Adafruit_PWMServoDriver.h>

    #include "../utils/log.hpp"

    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire1);
    
    #define SERVOMIN 150
    #define SERVOMAX 600
    #define FREQ 60

    uint8_t servonum = 0;

    void setup() {
        SerialUSB.begin(9600);

        logln("External pwm driver test");

        pwm.begin();
        pwm.setPWMFreq(FREQ);
    }

    void loop() {
        log("Servo: ") logln(servonum);

        logln("Moving forward");

        for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
            pwm.setPWM(servonum, 0, pulselen);
        }

        delay(500);

        logln("Moving backward");
        
        for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
            pwm.setPWM(servonum, 0, pulselen);
        }

        delay(500);

        servonum ++;
        if (servonum > 15) servonum = 0;
    }

    #endif