#pragma once

#include <menu.h>

template<class TResult>
struct MenuItemsResult {
    MenuItemsResult(TResult** items, const unsigned short length):
        items(items), length(length) {}
        
    TResult** items;
    unsigned short length;
};
