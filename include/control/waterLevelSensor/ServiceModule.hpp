#pragma once

#include "../../utils/log.hpp"

#include "WaterLevelSensor.hpp"

#include "../../modules/serviceMode/ServiceModule.hpp"

#include "../../menu/YesNoToggle.hpp"

#include "../../system/PreciseTimer.hpp"


class WaterLevelSensorServiceModule: public ServiceModuleWithHelpers {
	public:
		struct SensorSetup {
			char* label;
			unsigned short deviceId;
			unsigned short waterLevel;
		};

	private:
		WaterLevelSensor* waterLevelSensor;

		SensorSetup* setup;
		unsigned short numberOfSensors = 0;

		bool isMenuEntered = false;
		bool* sensorData = nullptr;

		PreciseTimer senseTimer;
		unsigned short sensorCounter = 0;

	protected:
		void menuEntered() {
			isMenuEntered = true;
		}

		void menuLeaved() {
			isMenuEntered = false;
		}

	public:
		WaterLevelSensorServiceModule(
			WaterLevelSensor* waterLevelSensor,
			unsigned short numberOfSensors,
			SensorSetup* setup
		):
			waterLevelSensor(waterLevelSensor),
			numberOfSensors(numberOfSensors),
			setup(setup),
			sensorData(new bool[numberOfSensors] { false })
		{}

		prompt* getServiceMenu() {
			prompt** items = new prompt*[numberOfSensors] { nullptr };

			for (unsigned short i = 0; i < numberOfSensors; ++i) {
				items[i] = YesNoToggle(setup[i].label, sensorData[i], nullptr);
				items[i]->disable();
			}

			return new menuNode("Sensory", numberOfSensors, items, menuStateChangeEvent, (eventMask)(enterEvent | exitEvent));
		}

		void update(const RtcDateTime &time) {
			if (!isMenuEntered || !senseTimer.done()) return;

			senseTimer.start(100);

			if (sensorCounter >= numberOfSensors) {
				sensorCounter = 0;
			} else {
				sensorCounter += 1;
			}

			sensorData[sensorCounter] = waterLevelSensor->sense(setup[sensorCounter].deviceId, setup[sensorCounter].waterLevel);
		}
};