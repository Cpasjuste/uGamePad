//
// Created by cpasjuste on 29/03/23.
//

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "main.h"

using uGamePad::PicoGfx;

PicoGfx::PicoGfx() : Gfx({128, 64}) {
    // init OLED display
    Wire.setSDA(D12);
    Wire.setSCL(D13);
    p_display = new Adafruit_SSD1306(m_size.x, m_size.y, &Wire, -1);
    if (!((Adafruit_SSD1306 *) p_display)->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        printf("PicoGfx: SSD1306 init failed\r\n");
        return;
    }
    p_display->setTextWrap(false);
}

void PicoGfx::flip() {
    if (p_display) ((Adafruit_SSD1306 *) p_display)->display();
}
