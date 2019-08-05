#pragma once

#include <RtcDateTime.h>

#include "../system/SystemTime.hpp"

bool isInTimeScope(const SystemTime& start, const SystemTime& end, const SystemTime& target) {
    if (start == end) return false;
    else if (end > start) return target > start && target < end;
    else if (start > end) return target > start || target < end;
    else return false;
}

bool isInTimeScope(const SystemTime& start, const SystemTime& end, const RtcDateTime& target) {
    return isInTimeScope(start, end, asSystemTime(target));
}