#pragma once

#include <RtcDateTime.h>

#define ONE_MINUTE 60
#define ONE_HOUR (60 * 60)
#define ONE_DAY (24 * 60 * 60)

struct SystemTime;
unsigned int asSeconds(const SystemTime& time);
SystemTime asSystemTime(const RtcDateTime& time);
unsigned int cutToOneDay(const unsigned int& seconds);
unsigned short cutToOneHour(const unsigned int& seconds);

struct SystemTime {
    unsigned short hour;
    unsigned short minute;

    friend bool operator == (const SystemTime& left, const SystemTime& right) {
        return asSeconds(left) == asSeconds(right);
    }

    friend bool operator == (const SystemTime& left, const RtcDateTime& right) {
        return asSeconds(left) == asSeconds(asSystemTime(right));
    }

	friend bool operator != (const SystemTime& left, const SystemTime& right) {
        return asSeconds(left) != asSeconds(right);
    }

    friend bool operator != (const SystemTime& left, const RtcDateTime& right) {
        return asSeconds(left) != asSeconds(asSystemTime(right));
    }

    friend bool operator < (const SystemTime& left, const RtcDateTime& right) {
		return asSeconds(left) < asSeconds(asSystemTime(right));
    }

    friend bool operator > (const SystemTime& left, const RtcDateTime& right) {
		return asSeconds(left) > asSeconds(asSystemTime(right));
    }

    friend bool operator < (const SystemTime& left, const SystemTime& right) {
		return asSeconds(left) < asSeconds(right);
    }

    friend bool operator > (const SystemTime& left, const SystemTime& right) {
		return asSeconds(left) > asSeconds(right);
    }
	
	friend bool operator <= (const SystemTime& left, const RtcDateTime& right) {
		return asSeconds(left) <= asSeconds(asSystemTime(right));
    }

    friend bool operator >= (const SystemTime& left, const RtcDateTime& right) {
		return asSeconds(left) >= asSeconds(asSystemTime(right));
    }

    friend bool operator <= (const SystemTime& left, const SystemTime& right) {
		return asSeconds(left) <= asSeconds(right);
    }

    friend bool operator >= (const SystemTime& left, const SystemTime& right) {
		return asSeconds(left) >= asSeconds(right);
    }

	friend SystemTime operator + (const SystemTime& left, const SystemTime& right) {
		return asSystemTime(asSeconds(left) + asSeconds(right));
	}

	friend SystemTime operator - (const SystemTime& left, const SystemTime& right) {
		if (left > right) {
			return asSystemTime(asSeconds(left) - asSeconds(right));
		} else {
			return asSystemTime(
				cutToOneDay(
					asSeconds(left) + (ONE_DAY * ((asSeconds(right) / ONE_DAY + 1)) - asSeconds(right))
				)
			);
		}
	}

	friend void operator += (SystemTime& left, const SystemTime& right) {
		left = left + right;
	}

	friend void operator -= (SystemTime& left, const SystemTime& right) {
		left = left - right;
	}
};

SystemTime asSystemTime(const RtcDateTime& time) {
    return {
        .hour = time.Hour(),
        .minute = time.Minute()
    };
}

unsigned int asSeconds(const SystemTime& time) {
    unsigned int totalMinutes = time.hour * 60 + time.minute;
    return totalMinutes * 60;
}

SystemTime asSystemTime(const unsigned int& seconds) {
	const unsigned int resultSeconds = cutToOneDay(seconds);
	
	SystemTime result {
		.hour = resultSeconds / ONE_HOUR,
		.minute = cutToOneHour(resultSeconds) / ONE_MINUTE
	};

	return result;
}

unsigned int distanceBetween(const unsigned int& t1Seconds, const unsigned int& t2Seconds) {
	return cutToOneDay(ONE_DAY - t1Seconds + t2Seconds);
}

unsigned int distanceBetween(const SystemTime& t1, const SystemTime& t2) {
	return distanceBetween(asSeconds(t1), asSeconds(t2));
}

unsigned int distanceBetween(const SystemTime& t1, const unsigned int& t2Seconds) {
	return distanceBetween(asSeconds(t1), t2Seconds);
}

unsigned int distanceBetween(const unsigned int& t1Seconds, const SystemTime& t2) {
	return distanceBetween(t1Seconds, asSeconds(t2));
}

unsigned int absoluteDistanceBetween(const unsigned int& t1Seconds, const unsigned int& t2Seconds) {
	const unsigned int
		firstDistance = distanceBetween(t1Seconds, t2Seconds),
		secondDistance = distanceBetween(t2Seconds, t1Seconds);

	return firstDistance > secondDistance
		? secondDistance
		: firstDistance;
}

unsigned int absoluteDistanceBetween(const SystemTime& t1, const SystemTime& t2) {
	return absoluteDistanceBetween(asSeconds(t1), asSeconds(t2));
}

unsigned int absoluteDistanceBetween(const SystemTime& t1, const unsigned int& t2Seconds) {
	return absoluteDistanceBetween(asSeconds(t1), t2Seconds);
}

unsigned int absoluteDistanceBetween(const unsigned int& t1Seconds, const SystemTime& t2) {
	return absoluteDistanceBetween(t1Seconds, asSeconds(t2));
}


unsigned int cutToOneDay(const unsigned int& seconds) {
	if (seconds < ONE_DAY) {
		return seconds;
	}

	const unsigned short numberOfDaysIncludedInSelectedTime = seconds / ONE_DAY;
	return seconds - ONE_DAY * numberOfDaysIncludedInSelectedTime;
}

unsigned short cutToOneHour(const unsigned int& seconds) {
	if (seconds < ONE_HOUR) {
		return seconds;
	}

	const unsigned short numberOfHoursIncludedInSelectedTime = seconds / ONE_HOUR;
	return seconds - ONE_HOUR * numberOfHoursIncludedInSelectedTime;
}