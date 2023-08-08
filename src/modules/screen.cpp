// Librerías de control de pantalla
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <modules/text_alignment.h>

// Declarando carateristica de pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define ITEM_MAX 8

int lines[3] = {5, 3, 2};
int interlines[3] = {8, 16, 48};
int maxCharacters[3] = {21, 10, 6};

struct Text {
  const char* text;
  int16_t x;
  int16_t y;
  uint8_t textSize;
};

uint8_t itemCount = 0;
Text itemList[ITEM_MAX];


Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void putText(const Text& item) {
  uint8_t x, y;
  
  // Alinear text en X
  if (item.x == TEXT_CENTER) {
    x = (SCREEN_WIDTH - (int16_t)(strlen(item.text) * (SCREEN_WIDTH / maxCharacters[item.textSize - 1]))) / 2;
  } else if (item.x == TEXT_LEFT) {
    x = 0;
  } else if (item.x == TEXT_RIGHT) {
    x = SCREEN_WIDTH - (strlen(item.text) * (uint16_t)(SCREEN_WIDTH /maxCharacters[item.textSize - 1]));
  } else {
    x = item.x;
  }

  // Alinear text en Y
  if (item.y == TEXT_MIDDLE) {
    y = (SCREEN_HEIGHT / 2) - (interlines[item.textSize - 1] / 2);
  } else if (item.y == TEXT_TOP) {
    y = 0;
  } else if (item.y == TEXT_BOTTOM) {
    y = SCREEN_HEIGHT - interlines[item.textSize - 1];
  } else {
    y = item.y;
  }

  oled.setTextSize(item.textSize);
  oled.setCursor(x, y);
  oled.print(item.text);
  // oled.print(x);
}


void screenPrint(const char text[], int16_t x, int16_t y, uint8_t textSize = 1) {    
  if (itemCount < ITEM_MAX) {
    itemList[itemCount].text = text;
    itemList[itemCount].x = x;
    itemList[itemCount].y = y;
    itemList[itemCount].textSize = textSize;

    itemCount++;
  }
}


void screenShow() {
  oled.clearDisplay();

  for (int i = 0; i < itemCount; i++) {
    putText(itemList[i]);
  }

  memset(itemList, 0, itemCount);
  itemCount = 0;

  oled.display();
}


void screenInit() {
  // Mostrar para depurar por si no funcionó la pantalla
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("La pantalla OLED falló");

    while (true);
  }

  // Esperemos a que inicie la pantalla
  delay(100);

  // Usamos el color blanco por defecto
  oled.setTextColor(WHITE);

  screenPrint("Iniciando...", TEXT_CENTER, TEXT_CENTER);
  screenShow();
}