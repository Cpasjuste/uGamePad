//
// Created by cpasjuste on 30/03/23.
//

#include "platform.h"

using namespace uGamePad;

void Platform::setup() {
    p_ui = new Ui();

    fflush(stdout);

    // motd
    printf("        _____                      _____          _ \r\n"
        "       / ____|                    |  __ \\        | |\r\n"
        " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
        "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
        "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
        " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n\n");

    fflush(stdout);

#if 0 // TODO
    auto info = p_fs->getDeviceInfo();
    printf("Platform: filesystem size: %lu Bytes, used: %lu Bytes (available: %i)\r\n",
           info.totalBytes, info.usedBytes, p_fs->isAvailable() ? 1 : 0);
    printf("Platform: heap size: %i\r\n", getFreeHeap());
#endif
}

void Platform::loop() {
    // loops
    p_hid->loop();
    p_pad->loop();
    p_ui->loop();
}

Platform::~Platform() {
    delete (p_gfx);
    delete (p_hid);
    delete (p_pad);
    delete (p_config);
    delete (p_fs);
}
