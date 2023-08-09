#include <Arduino.h>

#include <modules/text_alignment.h>
#include <modules/screen.h>
#include <modules/thermistor.h>

// SSR
#define PIN_SSR 9

// Termistor
#define PIN_NTC A0

// Buzzer
#define PIN_BUZZER 6

// Buttons
#define BUTTON_LEFT 9
#define BUTTON_RIGHT 10
#define BUTTON_A 11
#define BUTTON_B 12

char ssrState[] = "OFF";

void mainMenu() {
  // Seleccione Modo
  // Modo Temperatura Constante
  // - 40°C
  // - 60°C
  // - 80°C
  // - 100°C
  // Modo Soldadura 138°C
  // Modo Soldadura 183°C
  // Modo Test
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_SSR, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  Serial.println("Iniciando...");
  screenInit();

  tone(PIN_BUZZER, 1800, 500);
  delay(1000);
}

int secs = 0;

void loop() {
  int samples = 50;
  float average = 0.0;

  for (int i = 0; i < samples; i++) {
    average += thermistorTemperature(PIN_NTC);
    delay(5);
  }

  // float temp = calculateTemperature(PIN_NTC);
  float temp = average / samples;

  Serial.print("T: ");
  Serial.println(temp);

  // Formatear el valor float como cadena
  char buffer[10];  // Ajusta el tamaño del búfer según tus necesidades
  dtostrf(temp, 4, 2, buffer);  // Formatear el valor con 4 dígitos en total y 2 decimales

  // Construir la cadena completa
  char fullText[20];  // Ajusta el tamaño del búfer según tus necesidades
  snprintf(fullText, sizeof(fullText), "T: %s'C", buffer);

  screenPrint(("T: " + String(temp, 2) + "'C").c_str(), TEXT_LEFT, TEXT_TOP);
  screenPrint(fullText, TEXT_LEFT, TEXT_TOP);

  if (strcmp(ssrState, "OFF") == 0) {
    strcpy(ssrState, " ON");
  } else {
    strcpy(ssrState, "OFF");
  }

  char ssrDisplay[20]; // Ajusta el tamaño según tus necesidades
  snprintf(ssrDisplay, sizeof(ssrDisplay), "SSR:%s", ssrState);
  screenPrint(ssrDisplay, TEXT_RIGHT, TEXT_TOP);

  screenPrint("MODO X", TEXT_CENTER, TEXT_MIDDLE, 2);
  screenPrint("Corriendo prueba", TEXT_CENTER, 42);

  screenPrint("<", TEXT_LEFT, TEXT_MIDDLE, 2);
  screenPrint(">", TEXT_RIGHT, TEXT_MIDDLE, 2);

  screenPrint("Cancelar", TEXT_LEFT, TEXT_BOTTOM);
  screenPrint("Aceptar", TEXT_RIGHT, TEXT_BOTTOM);

  screenShow();

  delay(1000);
}
