//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"

using namespace uGamePad;

Platform *platform;

Platform *getPlatform() {
    return platform;
}

void setup() {
#if NATIVE
    platform = new LinuxPlatform();
#else
    platform = new PicoPlatform();
#endif
    platform->setup();
}

void loop() {
    platform->loop();
}
