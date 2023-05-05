//
// Created by cpasjuste on 30/03/23.
//

#include "main.h"

using namespace uGamePad;

void Platform::setup() {
    p_ui = new Ui();

    // motd
    printf("        _____                      _____          _ \r\n"
           "       / ____|                    |  __ \\        | |\r\n"
           " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
           "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
           "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
           " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n\n");
    fflush(stdout);

    auto info = p_fs->getDeviceInfo();
    printf("Platform: filesystem size: %llu Bytes, used: %llu Bytes (available: %i)\r\n",
           info.totalBytes, info.usedBytes, p_fs->isAvailable());
    printf("Platform: heap size: %i\r\n", getFreeHeap());
}

void uGamePad::Platform::loop() {
    if (p_pad) p_pad->loop();
    if (p_ui && p_ui->isActive()) {
        p_gfx->clear();
        p_ui->loop();
        p_gfx->flip();
    }
}
