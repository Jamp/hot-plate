#include <Arduino.h>

uint8_t pinBuzzer;

void buzzerInit(uint8_t pin) {
  pinBuzzer = pin;

  pinMode(pinBuzzer, OUTPUT);
  digitalWrite(pinBuzzer, LOW);

  tone(pinBuzzer, 1800, 500);
}

void moveSound() {
  tone(pinBuzzer, 1500, 80);
}

void selectSound() {
  tone(pinBuzzer, 2300, 40);
}

void finishSound() {
  tone(pinBuzzer, 2500, 150);
  delay(130);
  tone(pinBuzzer, 2200, 150);
  delay(130);
  tone(pinBuzzer, 2000, 150);
  delay(130);
}

void runningSound() {

}
