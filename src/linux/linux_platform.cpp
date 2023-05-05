//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"
#include "linux_platform.h"

using namespace uGamePad;

void LinuxPlatform::setup() {
    // init filesystem
    p_fs = new Fs();

    // init display
    p_gfx = new LinuxGfx();

    // init gamepad
    p_pad = new LinuxGamePad();

    // all done
    Platform::setup();
}
