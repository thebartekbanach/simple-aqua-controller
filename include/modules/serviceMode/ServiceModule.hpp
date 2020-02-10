#pragma once

#include <RtcDS1302.h>
#include <menu.h>

#include "../../system/ActionReceiver.hpp"

class ServiceModule {
	public:
		virtual prompt* getServiceMenu() = 0;
		virtual void update(const RtcDateTime &time) { }
		
		virtual void activateModule() { }
		virtual void deactivateModule() { }
};

class ServiceModuleWithHelpers: public ServiceModule {
	private:
		void menuStateChange(eventMask event, navNode& nav, prompt &item) {
			switch (event) {
				case eventMask::enterEvent:
					menuEntered();
					break;
				
				case eventMask::exitEvent:
					menuLeaved();
					break;

				default: break;
			}
		}

	protected:
		ExactActionReceiver<ServiceModuleWithHelpers>* menuStateChangeEvent;

		ServiceModuleWithHelpers():
			menuStateChangeEvent(
				new ExactActionReceiver<ServiceModuleWithHelpers>(
					this, &ServiceModuleWithHelpers::menuStateChange
				)
			) {}

		virtual void menuEntered() = 0;
		virtual void menuLeaved() = 0;
};