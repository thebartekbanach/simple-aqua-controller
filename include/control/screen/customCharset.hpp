#pragma once

#include <Arduino.h>

#define arrowTopCustomChar 1
int arrowTopCharDefinition[] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00000
};

#define arrowDownCustomChar 2
int arrowDownCharDefinition[] = {
  B00000,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
  B00000
};

#define doubleVerticalArrowCustomChar 3
int doubleVerticalArrowCharDefinition[] = {
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
  B10001,
  B01010,
  B00100
};

#define arrowLeftCustomChar 4
int arrowLeftCharDefinition[] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

#define arrowRightCustomChar 5
int arrowRightCharDefinition[] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};