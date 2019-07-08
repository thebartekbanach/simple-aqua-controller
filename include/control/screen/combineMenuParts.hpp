#pragma once

#include <menu.h>

prompt** combineMenuParts(prompt** firstPart, size_t firstPartSize, prompt** secondPart, size_t secondPartSize) {
    prompt** total = (prompt**)malloc(sizeof(firstPart[0]) * firstPartSize + sizeof(secondPart[0]) * secondPartSize);

    memcpy(total, firstPart, sizeof(firstPart[0]) * firstPartSize);
    memcpy(total + firstPartSize, secondPart, sizeof(secondPart[0]) * secondPartSize);

    return total;
}

prompt** combineMenuParts(prompt** firstPart, size_t firstPartSize, prompt* addionalElement) {
    prompt* addionalElements[] { addionalElement };
    return combineMenuParts(firstPart, firstPartSize, addionalElements, 1);
}