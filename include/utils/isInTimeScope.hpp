#pragma once

#include <RtcDateTime.h>

#include "../system/SystemTime.hpp"

bool isInTimeScope(const SystemTime& start, const SystemTime& end, const SystemTime& target) {
    if (start == end) return false;
    else if (end > start && target > start && target < end) return true;
    else if (start > end && target > start || target < end) return true;
    else return false;
}

bool isInTimeScope(const SystemTime& start, const SystemTime& end, const RtcDateTime& target) {
    return isInTimeScope(start, end, asSystemTime(target));
}