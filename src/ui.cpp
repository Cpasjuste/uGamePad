//
// Created by cpasjuste on 28/03/23.
//

#include "splash.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "main.h"
#include "ui.h"

using uGamePad::Ui;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// SSD1306 I2C display (SDA, SCL)
#define OLED_RESET     {-1} // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

extern unsigned char splash_128x64 PROGMEM[];

void Ui::init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        printf("Ui::init: SSD1306 init failed\r\n");
        return;
    }

    display.drawBitmap(0, 0, splash_128x64, 128, 64, WHITE);

    display.setTextColor(WHITE);
    display.setCursor(42, 56);
    display.print("uGamePad");
    display.display();
}
