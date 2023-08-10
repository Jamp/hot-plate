#ifndef OLED_H
#define OLED_H

void oledInit();
void oledPrint(const char text[], int16_t x, int16_t y, uint8_t textSize = 1);
void oledShow();

#endif
