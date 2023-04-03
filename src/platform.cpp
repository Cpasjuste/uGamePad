//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"

using namespace uGamePad;

void Platform::setup() {
    p_ui = new Ui();
}

void uGamePad::Platform::loop() {
    p_gfx->clear();
    p_ui->loop();
    p_gfx->flip();
}
