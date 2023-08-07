#include <Arduino.h>
#include <math.h>

#define R1 4700.0
#define RESOLUTION 1023.0

float thermistorTemperature(uint8_t pin) {
  uint16_t Vo = analogRead(pin);

  float Beta = 4060.79;
  // float roomTemp = 294.55;  // 24°C
  float roomTemp = 296.15;  // 23°C
  float Ro = 100000.0;
  // print(f"Beta: {Beta} -  ")

  // R2 = R1 * (1_023.0 / Vo - 1.0)
  float R2 = ((Vo * R1) / (Vo - 1023.0) * -1);
  float temperatureKelvin = (Beta * roomTemp) / (Beta + (roomTemp * log(R2 / Ro)));
  float temperatureCelsius = temperatureKelvin - 273.15;

  return temperatureCelsius;
}  // getTemperature
