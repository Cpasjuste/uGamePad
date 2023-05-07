//
// Created by cpasjuste on 24/03/23.
//

#include <SDL2/SDL.h>
#include "linux_gamepad.h"

using namespace uGamePad;

// fake/testing output mode
static GamePad::Output output = {
        .name = "Jamma",
        .mode = GamePad::Mode::Jamma,
        .mappings = {
                {GamePad::Button::B1,     SDL_SCANCODE_1,      -1, -1},
                {GamePad::Button::B2,     SDL_SCANCODE_2,      -1, -1},
                {GamePad::Button::B3,     SDL_SCANCODE_3,      -1, -1},
                {GamePad::Button::B4,     SDL_SCANCODE_4,      -1, -1},
                {GamePad::Button::B5,     SDL_SCANCODE_5,      -1, -1},
                {GamePad::Button::B6,     SDL_SCANCODE_6,      -1, -1},
                {GamePad::Button::SELECT, SDL_SCANCODE_SPACE,  -1, -1},
                {GamePad::Button::START,  SDL_SCANCODE_RETURN, -1, -1},
                {GamePad::Button::UP,     SDL_SCANCODE_UP,     -1, -1},
                {GamePad::Button::DOWN,   SDL_SCANCODE_DOWN,   -1, -1},
                {GamePad::Button::LEFT,   SDL_SCANCODE_LEFT,   -1, -1},
                {GamePad::Button::RIGHT,  SDL_SCANCODE_RIGHT,  -1, -1},
        }
};

GamePad::Output *LinuxGamePad::getOutputMode() {
    return &output;
}

void LinuxGamePad::loop() {
    // check for quit event
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) exit(0);
    }

    // reset buttons state
    m_buttons = 0;

    // check for buttons (keyboard) press
    for (const auto &mapping: getOutputMode()->mappings) {
        m_buttons |= SDL_GetKeyboardState(nullptr)[mapping.pin] > 0 ? mapping.button : 0;
    }

    GamePad::loop();
}
