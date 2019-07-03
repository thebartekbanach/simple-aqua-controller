#pragma once

#include <menu.h>

template<class TResult>
struct MenuItemsResult {
    MenuItemsResult(TResult** items, const ushort length):
        items(items), length(length) {}
        
    TResult** items;
    ushort length;
};
