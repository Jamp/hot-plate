#ifndef SOLDER_H
#define SOLDER_H

#include <Arduino.h>

void initSolder(const float solderMode);
void solderProgram(uint8_t pinSSR, uint8_t pinNTC);

#endif