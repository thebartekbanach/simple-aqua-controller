#pragma once

#include <RtcDateTime.h>

struct SystemTime;
unsigned int asSeconds(const SystemTime& time);
SystemTime asSystemTime(const RtcDateTime& time);

struct SystemTime {
    unsigned short hour;
    unsigned short minute;

    friend bool operator == (const SystemTime& left, const SystemTime& right) {
        return asSeconds(left) == asSeconds(right);
    }

    friend bool operator == (const SystemTime& left, const RtcDateTime& right) {
        return asSeconds(left) == asSeconds(asSystemTime(right));
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

	friend SystemTime operator + (const SystemTime& left, const SystemTime& right) {
		return asSystemTime(asSeconds(left) + asSeconds(right));
	}

	friend SystemTime operator - (const SystemTime& left, const SystemTime& right) {
		if (left > right) {
			return asSystemTime(asSeconds(left) - asSeconds(right));
		} else {
			//			12		   +		24		 *		   13		 /		 24        + 1   -       13 = 12 + 24 * (0 + 1) - 13 = 36 - 13 = 23
			//			12		   +		24		 *		   25		 /		 24		   + 1	 - 		 25 = 12 + 24 * (1 + 1) - 25 = (60 - 25) % 24 = 35
			return asSystemTime((asSeconds(left) + ((24 * 60 * 60) * ((asSeconds(right) / (24 * 60 * 60) + 1)) - asSeconds(right))) % 24);
		}
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
	const unsigned int resultSeconds = seconds % (24 * 60 * 60);
	
	SystemTime result {
		.hour = resultSeconds / (60 * 60),
		.minute = (resultSeconds % (60 * 60)) / 60
	};

	return result;
}

SystemTime distanceBetween(const SystemTime& t1, const SystemTime& t2) {
	return asSystemTime((24 * 60 * 60 - asSeconds(t1) + asSeconds(t2)) % 24 * 60 * 60);
}

SystemTime shorterDistanceBetween(const SystemTime& t1, const SystemTime& t2) {
	return t1 > t2 ? distanceBetween(t2, t1) : distanceBetween(t1, t2);
}

SystemTime longerDistanceBetween(const SystemTime& t1, const SystemTime& t2) {
	return t1 > t2 ? distanceBetween(t1, t2) : distanceBetween(t2, t1);
}
