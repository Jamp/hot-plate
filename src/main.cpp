#include <Arduino.h>

#include <modules/thermistor.h>
#include <modules/screen.h>
#include <modules/buzzer.h>

#define MAX_STATE 3
#define MAX_TEMPERATURE 4

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

const float hysteresis = 2.0;
const int samples = 50;

// Modes
// Main Menu 0
// Set Heat 1
// Running Heat 2
// Waiting for Solder 138'C 3
// Running Solder 138'C 4
// Waiting for Solder 183'C 5
// Running Solder 183'C 6
int8_t mode = 0;

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

int temperatureSet = 0;
const float temperatureAvailable[5] = {
  40.0,
  60.0,
  80.0,
  100.0,
  200.0
};


void goMain() {
  mode = 0;
  mainState = 1;
  temperatureSet = 0;
}


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


void mainMenu(uint8_t pin) {
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

  // Move in menu
  if (!digitalRead(BUTTON_MINUS)) {
    moveSound();
    mainState--;
  } else if (!digitalRead(BUTTON_PLUS)) {
    moveSound();
    mainState++;
  } else if (!digitalRead(BUTTON_LEFT)) {
    // selectSound();
  } else if (!digitalRead(BUTTON_RIGHT)) {
    mode = mainState;
    finishSound();
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
}  // mainMenu


void heat(uint8_t pinThermistor, uint8_t pinSsr, float fixedTemperature) {
  const float hysteresis = 2.0;
  const int samples = 50;
  float average = 0.0;

  for (int i = 0; i < samples; i++) {
    average += thermistorTemperature(pinThermistor);
    delay(1);
  }

  float temperature = average / samples;

  // Control de la placa calefactora
  if (temperature < fixedTemperature - hysteresis) {
    digitalWrite(pinSsr, HIGH);  // Enciende la placa calefactora
  } else if (temperature > fixedTemperature + hysteresis) {
    digitalWrite(pinSsr, LOW);   // Apaga la placa calefactora
  }

  char buffer[10];
  char subTitle[20];
  dtostrf(fixedTemperature, 4, 2, buffer);
  snprintf(subTitle, sizeof(subTitle), "Fijo a %s'C", buffer);

  if (!digitalRead(BUTTON_LEFT)) {
    goMain();
    finishSound();
  } else if (!digitalRead(BUTTON_RIGHT)) {
    goMain();
    finishSound();
  }

  gui(
    temperature,
    ssrState,
    (uint8_t)mainState,
    subTitle,
    nullptr,
    nullptr,
    "Cancelar",
    "Cancelar"
  );

  delay(100);
}  // heat


void setHeat(uint8_t pinThermistor, uint8_t pinSsr) {
  const int samples = 50;
  float average = 0.0;

  for (int i = 0; i < samples; i++) {
    average += thermistorTemperature(pinThermistor);
    delay(1);
  }

  float temperature = average / samples;

  char buffer[10];
  char subTitle[20];
  dtostrf(temperatureAvailable[temperatureSet], 4, 2, buffer);
  snprintf(subTitle, sizeof(subTitle), "Fijar a %s'C", buffer);

  // Move on temperature
  if (!digitalRead(BUTTON_MINUS)) {
    moveSound();
    temperatureSet--;
  } else if (!digitalRead(BUTTON_PLUS)) {
    moveSound();
    temperatureSet++;
  } else if (!digitalRead(BUTTON_LEFT)) {
    goMain();
    selectSound();
  // } else if (!digitalRead(BUTTON_RIGHT)) {
  //   finishSound();
  }

  if (temperatureSet < 0) {
    temperatureSet = MAX_TEMPERATURE;
  } else if (temperatureSet > MAX_TEMPERATURE) {
    temperatureSet = 0;
  }

  gui(
    temperature,
    ssrState,
    (uint8_t)mainState,
    subTitle,
    "-",
    "+",
    "Cancelar",
    "Iniciar"
  );

  delay(100);
}  // setHeat

void loop() {
  switch (mode) {
    case 1:
      setHeat(PIN_NTC, PIN_SSR);
      break;

    case 2:
      heat(PIN_NTC, PIN_SSR, temperatureAvailable[temperatureSet]);
      break;
    
    default:
      mainMenu(PIN_NTC);
      break;
  }
}
