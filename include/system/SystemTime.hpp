#pragma once

#include <RtcDateTime.h>

struct SystemTime {
    unsigned short hour;
    unsigned short minute;

    friend bool operator == (const SystemTime& left, const SystemTime& right) {
        return left.hour == right.hour && left.minute == right.minute;
    }

    friend bool operator < (const SystemTime& left, const RtcDateTime& right) {
        if (right.Hour() != left.hour) return right.Hour() > left.hour;
        else return right.Minute() > left.minute;
    }

    friend bool operator > (const SystemTime& left, const RtcDateTime& right) {
        if (right.Hour() != left.hour) return right.Hour() < left.hour;
        else return right.Minute() < left.minute;
    }

    friend bool operator < (const SystemTime& left, const SystemTime& right) {
        if (right.hour != left.hour) return right.hour > left.hour;
        else return right.minute > left.minute;
    }

    friend bool operator > (const SystemTime& left, const SystemTime& right) {
        if (right.hour != left.hour) return right.hour < left.hour;
        else return right.minute < left.minute;
    }
};

SystemTime asSystemTime(const RtcDateTime& time) {
    return {
        .hour = time.Hour(),
        .minute = time.Minute()
    };
}

uint asSeconds(const SystemTime& time) {
    uint totalMinutes = time.hour * 60 + time.minute;
    return totalMinutes * 60;
}
