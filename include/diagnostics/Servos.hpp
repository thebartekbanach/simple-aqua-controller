#define SERVO_TEST_ENABLED false

#if SERVO_TEST_ENABLED == true
    #define DIAGNOSTICS_MODE

    #include <Arduino.h>
    #include <Servo.h>

    #include "../control/valves/devices.hpp"
    
    Servo aquarium;
    Servo addionalWaterTank;
    Servo cleanWater;
    Servo sewageWater;

    bool open = false;

	void setup()
	{
        aquarium.attach(AQUARIUM_WATER_VALVE_PIN);
        addionalWaterTank.attach(ADDIONAL_WATER_TANK_VALVE_PIN);
        cleanWater.attach(CLEAN_WATER_VALVE_PIN);
        sewageWater.attach(SEWAGE_WATER_VALVE_PIN);
    }

    void loop() 
    {
        aquarium.write(open ? SERVOS_OPEN_ANGLE : SERVOS_CLOSE_ANGLE);
        addionalWaterTank.write(open ? SERVOS_OPEN_ANGLE : SERVOS_CLOSE_ANGLE);
        cleanWater.write(open ? SERVOS_OPEN_ANGLE : SERVOS_CLOSE_ANGLE);
        sewageWater.write(open ? SERVOS_OPEN_ANGLE : SERVOS_CLOSE_ANGLE);

        open = !open;
        delay(2000);
    }

#endif