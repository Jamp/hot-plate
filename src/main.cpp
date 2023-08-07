#include <Arduino.h>

#include <modules/text_alignment.h>
#include <modules/screen.h>
#include <modules/thermistor.h>

// SSR
#define PIN_SSR 9
// Termistor
#define PIN_NTC A0

int pwm = 255;  // 30%

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

  // Iniciando pantalla
  Serial.println("Iniciando...");
  screenInit();

  delay(2000);
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

  screenPrint(("T: " + String(temp, 2) + "'C").c_str(), TEXT_LEFT, TEXT_TOP);

  // oled.setCursor(0, 0);
  // oled.print("T: " + String(temp, 2) + "'C");
  // oled.setCursor(30, 0);
  // oled.print(temp, 2);
  // oled.setCursor(85, 0);
  // oled.print("'C");
  
  // oled.setCursor(0, verticalLine * 1);
  // oled.print("PWM");
  // oled.setCursor(50, verticalLine * 1);
  // oled.print(pwm);

  // oled.setCursor(0, verticalLine * 2);
  // oled.print("Time:");
  // oled.setCursor(60, verticalLine * 2);
  // oled.print(secs);

  // oled.setCursor(0, 0);
  // oled.print("Temp:");
  // oled.setCursor(0, 20);
  // oled.print(temp);
  
  // analogWrite(PIN_SSR, pwm);
  digitalWrite(PIN_SSR, LOW);

  // if (temp > 210) {
  //   digitalWrite(PIN_SSR, LOW);
  // } else {
  //   secs++;
  //   digitalWrite(PIN_SSR, HIGH);
  // }

  delay(1000);
}

// 40°C 
// 50°C 166947 Ohms
// 60°C 107563 Ohms
// 90°C 480
// 100°C 48615 Ohms
// 110°C 48566O

