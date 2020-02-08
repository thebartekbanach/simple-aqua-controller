#pragma once

#include "../program/TimeGuard.hpp"

#include "../system/SystemModulesList.hpp"

#include "../control/relayModule/RelayModule.hpp"
#include "../control/waterLevelSensor/WaterLevelSensor.hpp"
#include "../control/valves/ValveModuleUsingAdafruitPwmServoDriver.hpp"
#include "../control/pwmLightController/PwmLightControllerUsingInternalPwm.hpp"

#include "../control/pwmLightController/ServiceModule.hpp"

#include "timeSetup/Module.hpp"
#include "feedingControl/Module.hpp"
#include "waterChangeModule/Module.hpp"
#include "waterAdditionControl/Module.hpp"
#include "softLightingControl/Module.hpp"
#include "heaterControl/Module.hpp"
#include "serviceMode/Module.hpp"


SystemModulesList* getSystemModules(navRoot* navRootDependency, TimeGuard* timeGuard) {
    logln("Initializing system dependencies");

    logln("Initializing relayModule")
    
    RelayModule* relayModule = new RelayModule(NUMBER_OF_RELAYS,
        new unsigned short[NUMBER_OF_RELAYS] { MAIN_PUMP_PIN,      ADDIONAL_PUMP_PIN,      AERATION_PIN,       STERILIZATION_PIN,      HEATING_LAMP_PIN,   LIGHTING_PIN,       HEATER_PIN },
        new bool[NUMBER_OF_RELAYS] {           HIGH,               HIGH,                   HIGH,               HIGH,                   HIGH,               LOW,                LOW }
    );
    
    logln("Initializing waterLevelSensor")

    WaterLevelSensor* waterLevelSensor = new WaterLevelSensor(
        NUMBER_OF_WATER_LEVEL_SENSORS,
        // Water level sensors pinout arrays size definition
        new short unsigned int[NUMBER_OF_WATER_LEVEL_SENSORS] {
            NUMBER_OF_AQUARIUM_WATER_SENSORS,
            NUMBER_OF_ADDIONAL_WATER_TANK_SENSORS
        }, 
        // Pinout definition
        new short unsigned int*[NUMBER_OF_WATER_LEVEL_SENSORS] {
            new short unsigned int[NUMBER_OF_AQUARIUM_WATER_SENSORS] {
                NORMAL_WATER_LEVEL_PIN,
                CHANGE_WATER_LEVEL_PIN
            },
            new short unsigned int[NUMBER_OF_ADDIONAL_WATER_TANK_SENSORS] {
                ADDIONAL_WATER_TANK_MAX_LEVEL_PIN,
                ADDIONAL_WATER_TANK_MIN_LEVEL_PIN
            }
        }
    );

	logln("Initializing PwmLightController")

	PwmLightControllerUsingInternalPwm *lightController = new PwmLightControllerUsingInternalPwm();

    logln("Initializing valveModule")

    ValveModuleUsingAdafruitPwmServoDriver* valveModule = new ValveModuleUsingAdafruitPwmServoDriver(
		ADAFRUIT_SERVO_DRIVER_ADDRESS,
		ADAFRUIT_SERVO_DRIVER_WIRE,
		ADAFRUIT_SERVO_DRIVER_FREQ,
        ADAFRUIT_SERVO_DRIVER_SERVO_MAX,
        ADAFRUIT_SERVO_DRIVER_SERVO_MIN,
        NUMBER_OF_VALVES,
        SERVOS_OPEN_ANGLE,
        SERVOS_CLOSE_ANGLE,
        REMOTES_DETECTION_PIN,

        // Servo close detection pinout
        new unsigned short[NUMBER_OF_VALVES] {
            AQUARIUM_WATER_VALVE_CLOSE_DETECTION_PIN,
            ADDIONAL_WATER_TANK_VALVE_CLOSE_DETECTION_PIN,
            CLEAN_WATER_VALVE_CLOSE_DETECTION_PIN,
            SEWAGE_WATER_VALVE_CLOSE_DETECTION_PIN
        }
    );

	logln("Initializing service modules")

	PwmLightControllerServiceModule* pwmLightControllerServiceModule = new PwmLightControllerServiceModule(lightController);

	#define NUMBER_OF_SERVICE_MODULES 1

	ServiceModule** serviceModules = new ServiceModule*[NUMBER_OF_SERVICE_MODULES] {
		pwmLightControllerServiceModule
	};

    logln("Initializing system modules")
    
    TimeSetupModule*                timeSetupModule =               new TimeSetupModule(timeGuard);
    FeedingControlModule*           feedingControlModule =          new FeedingControlModule(relayModule);
    WaterChangeModule*              waterChangeModule =             new WaterChangeModule(relayModule, waterLevelSensor, valveModule);
    WaterAdditionControlModule*     waterAdditionControlModule =    new WaterAdditionControlModule(relayModule, waterLevelSensor, valveModule);
    SoftLightingControlModule*      softLightingControlModule =     new SoftLightingControlModule(lightController, timeSetupModule);
    HeaterControlModule*            heaterControlModule =           new HeaterControlModule(relayModule);
    ServiceModeModule*              serviceModeModule =             new ServiceModeModule(serviceModules, NUMBER_OF_SERVICE_MODULES, navRootDependency);
	
	TimeScopedDeviceDriver*			heatingLampControlModule =		TimeScopedDeviceDriver::as("Lampa grzewcza", IN_DAY_CYCLE, heatingLamp, relayModule);
	TimeScopedDeviceDriver*			aerationControlModule =			TimeScopedDeviceDriver::as("Napowietrzanie", IN_NIGHT_CYCLE, aeration, relayModule);
	TimeScopedDeviceDriver*			sterilizationControlModule =	TimeScopedDeviceDriver::as("Sterylizacja", CONTINUOUS_ON, sterilization, relayModule);

    
    #define NUMBER_OF_MODULES 11

    SystemModule** modules = new SystemModule*[NUMBER_OF_MODULES] {
        timeSetupModule,
        feedingControlModule,
        waterChangeModule,
        waterAdditionControlModule,
        softLightingControlModule,
        heatingLampControlModule,
        aerationControlModule,
        heaterControlModule,
        sterilizationControlModule,
        valveModule,
        serviceModeModule
    };

    return new SystemModulesList(modules, NUMBER_OF_MODULES);
}