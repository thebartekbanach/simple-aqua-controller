#pragma once

#include "RelayModule.hpp"

#include "../../modules/serviceMode/ServiceModule.hpp"

#include "../../menu/OnOffToggle.hpp"


class RelayModuleServiceModule: public ServiceModule {
	private:
		RelayModule* relayModule = nullptr;
		char** deviceNames = nullptr;
		unsigned short numberOfDevices = 0;

		bool* deviceStates = nullptr;

		void turnOffAllDevices() {
			for (unsigned short i = 0; i < numberOfDevices; ++i) {
				relayModule->set(i, OFF);
			}
		}

		void stateChanged() {
			for (unsigned short i = 0; i < numberOfDevices; ++i) {
				relayModule->set(i, deviceStates[i]);
			}
		}

	public:
		RelayModuleServiceModule(RelayModule* relayModule, unsigned short numberOfDevices, char** deviceNames):
			relayModule(relayModule),
			deviceNames(deviceNames),
			numberOfDevices(numberOfDevices),
			deviceStates(new bool[numberOfDevices] { false }) {}
			
		void activateModule() {
			turnOffAllDevices();
		}

		void deactivateModule() {
			turnOffAllDevices();
		}

		prompt* getServiceMenu() {
			ActionReceiver<RelayModuleServiceModule>* updateAction =
				new ActionReceiver<RelayModuleServiceModule>(this, &RelayModuleServiceModule::stateChanged);

			prompt** items = new prompt*[numberOfDevices] { nullptr };

			for (unsigned short i = 0; i < numberOfDevices; ++i) {
				items[i] = OnOffToggle(deviceNames[i], deviceStates[i], updateAction, enterEvent);
			}

			return new menuNode("Urzadzenia", numberOfDevices, items);
		};
};