#pragma once

class PwmLightController {
	public:
		virtual void setLightPwm(const unsigned short &value) = 0;
};