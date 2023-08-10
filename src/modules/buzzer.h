#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

void buzzerInit(uint8_t pin);
void moveSound();
void selectSound();
void finishSound();
void runningSound();

#endif
