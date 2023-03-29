//
// Created by cpasjuste on 29/03/23.
//

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "pico_ui.h"

#define SCREEN_WIDTH 128 // display width, in pixels
#define SCREEN_HEIGHT 64 // display height, in pixels

using uGamePad::PicoUi;

PicoUi::PicoUi() : Ui({128, 64}) {
    p_display = new Adafruit_SSD1306(m_size.x, m_size.y, &Wire, -1);
    if (!((Adafruit_SSD1306 *) p_display)->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        printf("PicoUi: SSD1306 init failed\r\n");
    }
}

void PicoUi::flip() {
    if (p_display) ((Adafruit_SSD1306 *) p_display)->display();
}
