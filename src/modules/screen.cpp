#include <Arduino.h>

#include <modules/text_alignment.h>
#include <modules/oled.h>

void screenInit() {
  oledInit();
}

void gui(
  float temperature, const char* ssrState, const uint8_t mode, 
  const char* subTitle = nullptr, const char* minusOption = nullptr, 
  const char* plusOption = nullptr, const char* leftOption = nullptr,
  const char* rightOption = nullptr
) {
    // Formatear el valor float como cadena
  char buffer[10];  // Ajusta el tamaño del búfer según tus necesidades
  dtostrf(temperature, 4, 2, buffer);  // Formatear el valor con 4 dígitos en total y 2 decimales

  // Construir la cadena completa
  char temperatureText[20];  // Ajusta el tamaño del búfer según tus necesidades
  snprintf(temperatureText, sizeof(temperatureText), "T: %s'C", buffer);

  oledPrint(temperatureText, TEXT_LEFT, TEXT_TOP);

  char ssrText[20];
  snprintf(ssrText, sizeof(ssrText), "SSR:%s", ssrState);
  oledPrint(ssrText, TEXT_RIGHT, TEXT_TOP);

  char modeText[10];
  snprintf(modeText, sizeof(modeText), "MODO %d", mode);
  oledPrint(modeText, TEXT_CENTER, TEXT_MIDDLE, 2);

  if (subTitle != nullptr) {
    oledPrint(subTitle, TEXT_CENTER, 42);
  }

  if (minusOption != nullptr) {
    oledPrint(minusOption, TEXT_LEFT, TEXT_MIDDLE, 2);
  }

  if (plusOption != nullptr) {
    oledPrint(plusOption, TEXT_RIGHT, TEXT_MIDDLE, 2);
  }

  if (leftOption != nullptr) {
    oledPrint(leftOption, TEXT_LEFT, TEXT_BOTTOM);
  }

  if (rightOption != nullptr) {
    oledPrint(rightOption, TEXT_RIGHT, TEXT_BOTTOM);
  }

  oledShow();
}