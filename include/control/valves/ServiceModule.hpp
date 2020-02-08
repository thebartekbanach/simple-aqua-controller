#pragma once

#include "ValveModule.hpp"

#include "../../modules/serviceMode/ServiceModule.hpp"

#include "../../menu/OnOffToggle.hpp"
#include "../../menu/YesNoToggle.hpp"

class ServovalvesServiceModule: public ServiceModuleWithHelpers {
	private:
		ValveModule* valveModule = nullptr;

		bool servoIsOpen[4] = { false };
		bool lastServoIsOpenForCompare[4] = { false };
		bool servoIsDetectedAsClosed[4] = { false };
		bool remotesAreAvailable = false;

		bool menuIsOpen = false;

		void closeAllServovalves() {
			for (unsigned short i = 0; i < 4; ++i) {
				if (!valveModule->isClosed(i)) {
					valveModule->close(i);
				}
			}
		}

		void updateServoDetectedAsClosedValues() {
			for (unsigned short i = 0; i < 4; ++i) {
				servoIsDetectedAsClosed[i] = valveModule->isClosed(i);
			}
		}

		prompt* getServovalveControlMenu(const unsigned short &index, char* name) {
			ActionReceiver<ServovalvesServiceModule>* updateEvent =
				new ActionReceiver<ServovalvesServiceModule>(this, &ServovalvesServiceModule::servoStateChanged);

			prompt** items = new prompt*[2] {
				OnOffToggle("Status: ", servoIsOpen[index], updateEvent),
				OnOffToggle("Krancowka: ", servoIsDetectedAsClosed[index], nullptr)
			};

			items[1]->disable();

			return new menuNode(name, 2, items);
		}

		void servoStateChanged() {
			short changedServoIndex = -1;
			bool newState = false;

			for (unsigned short i = 0; i < 4; ++i) {
				if (servoIsOpen[i] != lastServoIsOpenForCompare[i]) {
					changedServoIndex = i;
					lastServoIsOpenForCompare[i] = servoIsOpen[i];
					newState = servoIsOpen[i];
					break;
				}
			}

			if (changedServoIndex == -1) return;

			valveModule->set(changedServoIndex, newState);
		}

	protected:
		void menuEntered() {
			menuIsOpen = true;
		}

		void menuLeaved() {
			menuIsOpen = false;
		}

	public:
		ServovalvesServiceModule(ValveModule* valveModule):
			valveModule(valveModule) {}

		void activateModule() {
			closeAllServovalves();
		}

		void deactivateModule() {
			closeAllServovalves();
		}
		
		prompt* getServiceMenu() {
			prompt** items = new prompt*[5] {
				getServovalveControlMenu(aquariumWaterValve, "Obieg akwarium"),
				getServovalveControlMenu(addionalWaterTankValve, "Zbiornik dolewki"),
				getServovalveControlMenu(cleanWaterValve, "Czysta woda"),
				getServovalveControlMenu(sewageWaterValve, "Scieki"),
				YesNoToggle("Zdalne zawory", remotesAreAvailable, nullptr)
			};

			return new menuNode("Serwozawory", 5, items);
		};

		void update(const RtcDateTime &time) {
			if (!menuIsOpen) return;

			remotesAreAvailable = valveModule->areRemotesAvailable();
			updateServoDetectedAsClosedValues();
		}
};