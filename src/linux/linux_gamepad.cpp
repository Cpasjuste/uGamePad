//
// Created by cpasjuste on 24/03/23.
//

#include <SDL2/SDL.h>
#include "linux_gamepad.h"

using namespace uGamePad;

LinuxGamePad::LinuxGamePad() : GamePad() {
    // for linux ui debug/testing
    for (int i = 0; i < 2; i++) {
        m_outputModes.push_back(
            {
                .name = "Jamma",
                .mode = Jamma,
                .mappings = {
                    {B1, SDL_SCANCODE_UNKNOWN},
                    {B2, SDL_SCANCODE_UNKNOWN},
                    {B3, SDL_SCANCODE_UNKNOWN},
                    {B4, SDL_SCANCODE_UNKNOWN},
                    {B5, SDL_SCANCODE_UNKNOWN},
                    {B6, SDL_SCANCODE_UNKNOWN},
                    {SELECT, SDL_SCANCODE_UNKNOWN},
                    {START, SDL_SCANCODE_UNKNOWN},
                    {UP, SDL_SCANCODE_UNKNOWN},
                    {DOWN, SDL_SCANCODE_UNKNOWN},
                    {LEFT, SDL_SCANCODE_UNKNOWN},
                    {RIGHT, SDL_SCANCODE_UNKNOWN},
                }
            }
        );
    }

    m_outputModes[1].name = "Nes";
    m_outputModes[1].mode = Nes;
    //m_outputModes[2].name = "Snes";
    //m_outputModes[2].mode = Snes;
    //m_outputModes[3].name = "MD";
    //m_outputModes[3].mode = Md;

    LinuxGamePad::setOutputMode(m_outputMode);
}

uint32_t LinuxGamePad::getHardwareButtons() {
    uint32_t buttons = 0;

    // check for quit event
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) exit(0);
    }

    // simulate hardware buttons
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_UP]) buttons |= DPAD_UP;
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DOWN]) buttons |= DPAD_DOWN;
    if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_KP_ENTER]) buttons |= MENU;

    return buttons;
}
