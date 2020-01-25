#pragma once
#include "../../system/PreciseTimer.hpp"
#include "../../system/SystemTime.hpp"
#include "../../utils/isInTimeScope.hpp"

class LightDriver {
	private:
		const unsigned short *maxChangeSpeed; // steps per second
		const unsigned short *maxLightLevelValue;
		const unsigned short *minLightLevelValue;

		float requestedLightLevel;
		float actualLightLevel;

		PreciseTimer lightLevelChangeTimer;

		float calculateLightLevelFor(
			const SystemTime &actualTime,
			const SystemTime &dayStart,
			const SystemTime &dayEnd,
			const SystemTime &middayStart,
			const SystemTime &middayEnd

		) {
			if (!isInTimeScope(dayStart, dayEnd, actualTime)) {
				return 0;
			}

			if (isInTimeScope(middayStart, middayEnd, actualTime)) {
				return 1;
			}

			if (isInTimeScope(dayStart, middayStart, actualTime)) {
				const unsigned int actualProgress = asSeconds(distanceBetween(dayStart, actualTime));
				const unsigned int fullProgress = asSeconds(distanceBetween(dayStart, middayStart));
				return (float)actualProgress / fullProgress;
			}

			if (isInTimeScope(middayEnd, dayEnd, actualTime)) {
				const unsigned int actualProgress = asSeconds(distanceBetween(middayEnd, actualTime));
				const unsigned int fullProgress = asSeconds(distanceBetween(middayEnd, dayEnd));
				return (float)actualProgress / fullProgress;
			}
		}

		unsigned short calculateAndGetLightLevel() {
			if (requestedLightLevel != actualLightLevel) {
				if (lightLevelChangeTimer.done()) {
					lightLevelChangeTimer.start(*maxChangeSpeed < 1000 ? 1000 / *maxChangeSpeed : 1);
					
					const float additionValue = *maxChangeSpeed <= 1000 ? 1 : (float)*maxChangeSpeed / 1000;
					actualLightLevel += actualLightLevel < requestedLightLevel ? additionValue : additionValue * -1;
				}
			}

			return actualLightLevel;
		}

	public:
		LightDriver(
			const unsigned short *maxChangeSpeed,
			const unsigned short *maxLightLevelValue,
			const unsigned short *minLightLevelValue):
				maxChangeSpeed(maxChangeSpeed),
				maxLightLevelValue(maxLightLevelValue),
				minLightLevelValue(minLightLevelValue) {}

		unsigned short update(
			const SystemTime &actualTime,
			const SystemTime &dayStart,
			const SystemTime &dayEnd,
			const SystemTime &middayStart,
			const SystemTime &middayEnd
		) {
			const float nextCalculatedLightingLevel = calculateLightLevelFor(
				actualTime,
				dayStart,
				dayEnd,
				middayStart,
				middayEnd
			);

			requestedLightLevel =
				(*maxLightLevelValue - *minLightLevelValue) * nextCalculatedLightingLevel
				+ *minLightLevelValue;

			return calculateAndGetLightLevel();
		}

		unsigned short turnLights(const bool &state) {
			requestedLightLevel = state == true ? *maxLightLevelValue : *minLightLevelValue;
			return calculateAndGetLightLevel();
		}
};
