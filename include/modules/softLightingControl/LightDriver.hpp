#pragma once
#include "../../system/PreciseTimer.hpp"
#include "../../system/SystemTime.hpp"
#include "../../utils/isInTimeScope.hpp"

class LightDriver {
	private:
		const unsigned short &maxChangeSpeed; // steps per second
		const unsigned short &maxLightLevelValue;
		const unsigned short &minLightLevelValue;

		float requestedLightLevel = 0;
		float actualLightLevel = 0;

		PreciseTimer lightLevelChangeTimer;

		float calculateLightLevelFor(
			const unsigned int &actualTime,
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
				const unsigned int actualProgress = distanceBetween(dayStart, actualTime);
				const unsigned int fullProgress = distanceBetween(dayStart, middayStart);

				return (float)actualProgress / fullProgress;
			}

			if (isInTimeScope(middayEnd, dayEnd, actualTime)) {
				const unsigned int actualProgress = distanceBetween(middayEnd, actualTime);
				const unsigned int fullProgress = distanceBetween(middayEnd, dayEnd);

				return 1 - (float)actualProgress / fullProgress;
			}

			return 0;
		}

		unsigned short calculateAndGetLightLevel() {
			if (requestedLightLevel != actualLightLevel) {
				if (lightLevelChangeTimer.done()) {
					lightLevelChangeTimer.start(maxChangeSpeed < 1000 ? 1000 / maxChangeSpeed : 1);
					
					const float additionValue = maxChangeSpeed <= 1000 ? 1 : (float)maxChangeSpeed / 1000;

					if (actualLightLevel < requestedLightLevel) {
						if (actualLightLevel + additionValue > requestedLightLevel) {
							actualLightLevel = requestedLightLevel;
						}
						else {
							actualLightLevel += additionValue;
						}
					}
					
					else { // if (actualLightLevel > requestedLightLevel)
						if (actualLightLevel - additionValue < requestedLightLevel) {
							actualLightLevel = requestedLightLevel;
						}
						else {
							actualLightLevel -= additionValue;
						}
					}
				}
			}

			return actualLightLevel;
		}


	public:
		LightDriver(
			const unsigned short &maxChangeSpeed,
			const unsigned short &maxLightLevelValue,
			const unsigned short &minLightLevelValue):
				maxChangeSpeed(maxChangeSpeed),
				maxLightLevelValue(maxLightLevelValue),
				minLightLevelValue(minLightLevelValue) {}

		unsigned short update(
			const unsigned int &actualTime,
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
				(maxLightLevelValue - minLightLevelValue) * nextCalculatedLightingLevel
				+ minLightLevelValue;

			return calculateAndGetLightLevel();
		}

		unsigned short turnLights(const bool &state) {
			requestedLightLevel = state == true ? maxLightLevelValue : minLightLevelValue;
			return calculateAndGetLightLevel();
		}

		void reset(const unsigned short defaultLightLevel = 0) {
			requestedLightLevel = actualLightLevel = defaultLightLevel;
		}
};
