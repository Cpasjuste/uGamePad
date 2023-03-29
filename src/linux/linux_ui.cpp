//
// Created by cpasjuste on 29/03/23.
//

#include "Adafruit_GFX_dummy_display.h"
#include "linux_ui.h"

using uGamePad::LinuxUi;

LinuxUi::LinuxUi() : Ui({128, 64}) {
    p_display = new Adafruit_GFX_dummy_display(m_size.x, m_size.y, 4);
}

void LinuxUi::flip() {
    if (p_display) ((Adafruit_GFX_dummy_display *) p_display)->display();
}
