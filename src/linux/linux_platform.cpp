//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"
#include "linux_platform.h"

using namespace uGamePad;

void LinuxPlatform::setup() {
    // init filesystem
    p_fs = new Fs();

    // init config
    p_config = new Config(p_fs);

    // init gamepad
    p_pad = new LinuxGamePad();

    // linux hid api
    p_hid = new LinuxHid();

    // init display
    p_gfx = new LinuxGfx();

    // all done
    Platform::setup();
}
