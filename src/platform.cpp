//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"

using uGamePad::Platform;

uGamePad::Platform::Platform() {
    //p_ui = new Ui();
}

void uGamePad::Platform::loop() {
    //p_ui->update();
    p_gfx->flip();
}
