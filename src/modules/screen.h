#ifndef SCREEN_H
#define SCREEN_H

void screenInit();
void gui(
  float temperature, const char* ssrState, const uint8_t mode, 
  const char* subTitle = nullptr, const char* minusOption = nullptr, 
  const char* plusOption = nullptr, const char* leftOption = nullptr,
  const char* rightOption = nullptr
);

#endif
