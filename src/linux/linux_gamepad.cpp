//
// Created by cpasjuste on 24/03/23.
//

#include <SDL2/SDL.h>
#include "linux_gamepad.h"

using namespace uGamePad;

LinuxGamePad::LinuxGamePad() : GamePad() {
    // for linux ui debug/testing
#ifndef TODO_NES_SNES_MD_CABLES
    for (int i = 0; i < 4; i++) {
#endif
    m_outputModes.push_back(
        {
            .name = "Jamma",
            .mode = Jamma,
            .mappings = {
                {B1, SDL_SCANCODE_KP_1},
                {B2, SDL_SCANCODE_KP_2},
                {B3, SDL_SCANCODE_KP_3},
                {B4, SDL_SCANCODE_KP_4},
                {B5, SDL_SCANCODE_KP_5},
                {B6, SDL_SCANCODE_KP_6},
                {SELECT, SDL_SCANCODE_SPACE},
                {START, SDL_SCANCODE_RETURN},
                {UP, SDL_SCANCODE_UP},
                {DOWN, SDL_SCANCODE_DOWN},
                {LEFT, SDL_SCANCODE_LEFT},
                {RIGHT, SDL_SCANCODE_RIGHT},
            }
        }
    );
#ifndef TODO_NES_SNES_MD_CABLES
    }

    m_outputModes[1].name = "Nes";
    m_outputModes[1].mode = Mode::Nes;
    m_outputModes[2].name = "Snes";
    m_outputModes[2].mode = Mode::Snes;
    m_outputModes[3].name = "MD";
    m_outputModes[3].mode = Mode::Md;
#endif

    LinuxGamePad::setOutputMode(m_outputMode);
}

void LinuxGamePad::loop() {
    // check for quit event
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) exit(0);
    }

    // check for buttons (keyboard) press
    for (const auto &mapping: getOutputMode()->mappings) {
        m_buttons |= SDL_GetKeyboardState(nullptr)[mapping.pin] > 0 ? mapping.button : 0;
    }

    GamePad::loop();
}
