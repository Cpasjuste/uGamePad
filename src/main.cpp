//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"

using namespace uGamePad;

Platform *platform;

Platform *getPlatform() {
    return platform;
}

int main() {
#if LINUX
    platform = new LinuxPlatform();
#else
    platform = new PicoPlatform();
#endif
    platform->setup();

    while (true) platform->loop();
}
