//
// Created by cpasjuste on 29/03/23.
//

#include "Adafruit_GFX_dummy_display.h"
#include "linux_gfx.h"

using uGamePad::LinuxGfx;

LinuxGfx::LinuxGfx() : Gfx({128, 64}) {
    p_display = new Adafruit_GFX_dummy_display(m_size.x, m_size.y, 4);
}

void LinuxGfx::flip() {
    if (p_display) ((Adafruit_GFX_dummy_display *) p_display)->display();
}
