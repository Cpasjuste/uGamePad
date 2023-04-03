//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"

using namespace uGamePad;

// trying to do as much static allocation as possible...
Ui ui;

uGamePad::Platform::Platform() {
    p_ui = &ui;
}

void uGamePad::Platform::loop() {
    p_gfx->clear();
    p_ui->loop();
    p_gfx->flip();
}
