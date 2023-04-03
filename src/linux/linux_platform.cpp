//
// Created by cpasjuste on 29/03/23.
//

#include "linux_platform.h"
#include "linux_gamepad.h"
#include "linux_gfx.h"

using namespace uGamePad;

LinuxPlatform::LinuxPlatform() : Platform() {
    // init display
    p_gfx = new LinuxGfx();
    // init gamepad
    p_pad = new LinuxGamePad();
}

void LinuxPlatform::loop() {
    p_pad->update(nullptr, 0);
    Platform::loop();
}
