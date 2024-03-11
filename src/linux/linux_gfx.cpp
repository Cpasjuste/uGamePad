//
// Created by cpasjuste on 29/03/23.
//

#include <Adafruit_GFX.h>
#include "main.h"
#include "linux_display.h"

using namespace uGamePad;

LinuxGfx::LinuxGfx() : Gfx({128, 64}) {
    // init OLED display
    p_display = new LinuxDisplay();
    p_display->setTextWrap(false);
}

void LinuxGfx::flip() {
    if (p_display) ((LinuxDisplay *) p_display)->flip();
}
