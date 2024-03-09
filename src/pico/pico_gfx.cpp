//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"
#include <Adafruit_GFX.h>
#include "pico_display.h"

using uGamePad::PicoGfx;

PicoGfx::PicoGfx() : Gfx({128, 64}) {
    // init OLED display
    p_display = new PicoDisplay();
    p_display->setTextWrap(false);
}

void PicoGfx::flip() {
    if (p_display) ((PicoDisplay *) p_display)->flip();
}
