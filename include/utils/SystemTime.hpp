#pragma once

#include <RtcDateTime.h>

struct SystemTime {
    unsigned short hour;
    unsigned short minute;
};

SystemTime asSystemTime(const RtcDateTime& time) {
    return {
        .hour = time.Hour(),
        .minute = time.Minute()
    };
}
