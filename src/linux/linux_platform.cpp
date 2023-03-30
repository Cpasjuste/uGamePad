//
// Created by cpasjuste on 29/03/23.
//

#include <SDL2/SDL.h>
#include "linux_platform.h"
#include "linux_gamepad.h"
#include "linux_gfx.h"

using uGamePad::LinuxPlatform;

LinuxPlatform::LinuxPlatform() : Platform() {
    // init display
    p_gfx = new LinuxGfx();

    // init gamepad
    p_pad = new LinuxGamePad();
}

void LinuxPlatform::loop() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) exit(0);
    }

    Platform::loop();
}
