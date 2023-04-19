//
// Created by cpasjuste on 29/03/23.
//

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "main.h"

#define SCREEN_WIDTH 128 // display width, in pixels
#define SCREEN_HEIGHT 64 // display height, in pixels

using uGamePad::PicoGfx;

PicoGfx::PicoGfx() : Gfx({128, 64}) {
#if UGP_V10
#if !defined(ARDUINO_SEEED_XIAO_RP2040)
    // use GP0/GP1 for OLED screen of pico / rp2040 zero
    Wire.setSDA(0);
    Wire.setSCL(1);
#endif
#endif

    // init OLED display
    p_display = new Adafruit_SSD1306(m_size.x, m_size.y, &Wire, -1);
    if (!((Adafruit_SSD1306 *) p_display)->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        printf("PicoGfx: SSD1306 init failed\r\n");
    }
}

void PicoGfx::flip() {
    if (p_display) ((Adafruit_SSD1306 *) p_display)->display();
}
