#include <Arduino.h>

#include <config.h>
#include <modules/thermistor.h>
#include <modules/screen.h>

#define SENSE 50  // Tiempo de sensado del termistor
#define REFRESH 500 // Tiempo de refrescado de la pantalla(.5s)

char ssrState[] = "OFF";

float preheatTemperature, soakTemperature, reflowTemperature;

int solderState = 0;
const char* solderStates[5] = {
  "Pre-heating",
  "Soaking",
  "Reflow",
  "Cool down"
  "Completed",
};

bool shutdown = false;
float temperature;
float secs = 0;
unsigned int start = 0;

unsigned int timer1, timer2;
uint8_t stateSSR, modeSolder;
unsigned long timer1, start;

void initSolder(const float solderMode) {
  timer1 = millis();
  start = millis();

  if (solderMode == 138) {
    modeSolder = 2;  // Ajustar para que sea desde un archivo general
    preheatTemperature = PREHEAT_TEMPERATURE_138 + HYSTERESIS;
    soakTemperature = SOAK_TEMPERATURE_138 + HYSTERESIS;
    reflowTemperature = REFLOW_TEMPERATURE_138 + HYSTERESIS;
  } else {
    modeSolder = 3;  // Ajustar para que sea desde un archivo general
    preheatTemperature = PREHEAT_TEMPERATURE_183 + HYSTERESIS;
    soakTemperature = SOAK_TEMPERATURE_183 + HYSTERESIS;
    reflowTemperature = REFLOW_TEMPERATURE_183 + HYSTERESIS;
  }
}  // initSolder

void solderProgram(uint8_t pinSSR, uint8_t pinNTC) {
  start = millis();
  if (start - timer1 > SENSE) {
    timer1 = millis();

    temperature = thermistorTemperature(pinNTC);

    if (secs <= PREHEAT_TIME) {  // Preheat
      Serial.println("Preheat");
      solderState = 0;
      if (temperature < preheatTemperature) {
        stateSSR = HIGH;
      } else {
        stateSSR = LOW;
      }
    } else if (secs > PREHEAT_TIME & secs <= SOAK_TIME) {  // Soak
      Serial.println("Soak");
      solderState = 1;
      if (temperature < soakTemperature) {
        stateSSR = HIGH;
      } else {
        stateSSR = LOW;
      }
    } else if (secs > SOAK_TIME && secs < REFLOW_TIME) {  // Reflow
      Serial.println("Reflow");
      solderState = 2;
      if (temperature < reflowTemperature) {
        stateSSR = HIGH;
      } else {
        stateSSR = LOW;
      }
    } else if (secs >= REFLOW_TIME && temperature > SAFETY_TEMPERATURE) {   // Cooldown
      Serial.println("Cool down");
      solderState = 3;
      stateSSR = LOW;
    } else {
      solderState = 4;
      stateSSR = LOW;
    }

    // Control de los botones //
    // if (!digitalRead(BUTTON_MINUS)) {
    //   moveSound();
    //   temperatureSet--;
    // } else if (!digitalRead(BUTTON_PLUS)) {
    //   moveSound();
    //   temperatureSet++;
    // } else if (!digitalRead(BUTTON_LEFT)) {
    //   goMain();
    //   selectSound();
    // } else if (!digitalRead(BUTTON_RIGHT)) {
    //   finishSound();
    // }
    // Control de los botones  //

    digitalWrite(pinSSR, stateSSR);

    if (start - timer2 > REFRESH) {
      timer2 = millis();
      if (!shutdown) {
        secs += .5;  // Tiempo transcurrido .5s
      }

      char buffer[10];
      char timeRunning[20];
      dtostrf(secs, 4, 2, buffer);
      snprintf(timeRunning, sizeof(timeRunning), "Tiempo: %ss", buffer);

      // FIXME: centralizar esto
      if (stateSSR == LOW) {
        ssrState == "OFF";
      } else {
        ssrState == " ON";
      }

      // Actualizar pantalla
      gui(
        temperature,
        ssrState,
        modeSolder,
        solderStates[solderState],
        nullptr,
        nullptr,
        timeRunning,
        "Cancelar"
      );
    }
  }
}  // solder
