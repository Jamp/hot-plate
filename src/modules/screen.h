#ifndef SCREEN_H
#define SCREEN_H

void screenInit();
void screenPrint(const char text[], int16_t x, int16_t y, uint8_t textSize = 1);
void screenShow();

#endif
