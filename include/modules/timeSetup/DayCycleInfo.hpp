#pragma once

#include "../../system/SystemTime.hpp"

class DayCycleInfo {
	public:
		virtual const SystemTime& getDayStart() = 0;
		virtual const SystemTime& getNightStart() = 0;
};