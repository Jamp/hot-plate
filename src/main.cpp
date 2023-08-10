#include <Arduino.h>

#include <modules/thermistor.h>
#include <modules/screen.h>
#include <modules/buzzer.h>

#define MAX_STATE 3
#define MAX_TEMPERATURE 200.0

// Termistor
#define PIN_NTC A0

// SSR
#define PIN_SSR 5

// Buzzer
#define PIN_BUZZER 6

// Buttons
#define BUTTON_MINUS 9
#define BUTTON_PLUS 10
#define BUTTON_LEFT 11
#define BUTTON_RIGHT 12

// Seleccione Modo
// Modo Temperatura Constante (mainState = 1)
// Modo Soldadura 138°C (mainState = 2)
// Modo Soldadura 183°C (mainState = 3)
// Modo Test
int8_t mainState = 1;
const char* modeOptions[] = {
    "Temperatura Fija",
    "Soldadura 138'C",
    "Soldadura 183'C"
};

uint8_t secs = 0;
char ssrState[] = "OFF";

float temperatureFix = 40;
const float temperatureAvailable[5] = {
  40.0,
  60.0,
  80.0,
  100.0,
  200.0
};


char a[] = "NULL";

void setup() {
  Serial.begin(9600);
  pinMode(PIN_SSR, OUTPUT);

  pinMode(BUTTON_MINUS, INPUT_PULLUP);
  pinMode(BUTTON_PLUS, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

  Serial.println("Iniciando...");
  screenInit();

  buzzerInit(PIN_BUZZER);

  delay(1000);
}

void loop() {
  int samples = 50;
  float average = 0.0;

  for (int i = 0; i < samples; i++) {
    average += thermistorTemperature(PIN_NTC);
    delay(5);
  }

  // float temp = calculateTemperature(PIN_NTC);
  float temperature = average / samples;

  Serial.print("T: ");
  Serial.println(temperature);

  Serial.println("BUTTONS");
  Serial.println(digitalRead(BUTTON_MINUS));
  Serial.println(digitalRead(BUTTON_PLUS));
  Serial.println(digitalRead(BUTTON_LEFT));
  Serial.println(digitalRead(BUTTON_RIGHT));

  if (!digitalRead(BUTTON_MINUS)) {
    moveSound();
    strcpy(a, "<");
  } else if (!digitalRead(BUTTON_PLUS)) {
    moveSound();
    strcpy(a, ">");
  } else if (!digitalRead(BUTTON_LEFT)) {
    selectSound();
    strcpy(a, "+");
    mainState++;
  } else if (!digitalRead(BUTTON_RIGHT)) {
    finishSound();
    strcpy(a, "-");
    mainState--;
  }

  if (mainState < 1) {
    mainState = MAX_STATE;
  } else if (mainState > MAX_STATE) {
    mainState = 1;
  }

  gui(
    temperature,
    ssrState,
    (uint8_t)mainState,
    modeOptions[mainState - 1],
    "<",
    ">",
    nullptr,
    "Aceptar"
  );

  delay(10);
}
