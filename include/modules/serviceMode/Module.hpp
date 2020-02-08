#pragma once

#include "../../system/SystemModule.hpp"
#include "../../system/ActionReceiver.hpp"
#include "../../system/Timer.hpp"

#include "ServiceModule.hpp"

#include "Events.hpp"

class ServiceModeModule: public CommonSystemModule {
    private:
		ServiceModule** modules = nullptr;
		const unsigned short numberOfServiceModules = 0;

        navRoot* nav;

		bool serviceModeActive = false;
		int lastNavTimeout = 0;


		ExactActionReceiver<ServiceModeModule>* handleModeActivationAndDeactivation =
			new ExactActionReceiver<ServiceModeModule>(this, &ServiceModeModule::handleActivationAndDeactivation);

		void handleActivationAndDeactivation(eventMask event, navNode& nav, prompt &item) {
			switch (event) {
				case eventMask::enterEvent:
					activate();
					break;
				
				case eventMask::exitEvent:
					deactivate();
					break;

				default: break;
			}
		}

		void activate() {
			lastNavTimeout = nav->timeOut;
			nav->timeOut = 0;

			for (unsigned short i = 0; i < numberOfServiceModules; ++i) {
				modules[i]->activateModule();
			}

			serviceModeActive = true;
			eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_ACTIVATION);
		}

		void deactivate() {
			nav->timeOut = lastNavTimeout;

			for (unsigned short i = 0; i < numberOfServiceModules; ++i) {
				modules[i]->deactivateModule();
			}

			serviceModeActive = false;
			eventBus->send(SERVICE_MODE_MODULE_ID, SERVICE_MODE_DEACTIVATION);
		}

    public:
        ServiceModeModule(
			ServiceModule** modules,
			const unsigned short numberOfServiceModules,
            navRoot* nav):
				modules(modules),
				numberOfServiceModules(numberOfServiceModules),
                nav(nav) {}

        unsigned short getSettingsMenuItemsLength() { return 1; }

        menuNode** getSettingsMenuItems() {
			prompt** items = new prompt*[numberOfServiceModules] { nullptr };

			for (unsigned short i = 0; i < numberOfServiceModules; ++i) {
				items[i] = modules[i]->getServiceMenu();
			}

			return new menuNode*[1] {
				new menuNode(
					"Tryb serwisowy",
					numberOfServiceModules,
					items,
					handleModeActivationAndDeactivation,
					(eventMask)(enterEvent | exitEvent)
				)
			};
        }

        void update(const RtcDateTime& time) {
            if (!serviceModeActive) return;

			for (unsigned short i = 0; i < numberOfServiceModules; ++i) {
				modules[i]->update(time);
			}
        }
};