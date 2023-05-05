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
                {GamePad::Button::B1,     SDLK_a,               -1, -1},
                {GamePad::Button::B2,     SDLK_z,               -1, -1},
                {GamePad::Button::B3,     SDLK_e,               -1, -1},
                {GamePad::Button::B4,     SDLK_q,               -1, -1},
                {GamePad::Button::B5,     SDLK_s,               -1, -1},
                {GamePad::Button::B6,     SDLK_d,               -1, -1},
                {GamePad::Button::SELECT, SDLK_SPACE,           -1, -1},
                {GamePad::Button::START,  SDLK_RETURN,          -1, -1},
                {GamePad::Button::UP,     (uint8_t) SDLK_UP,    -1, -1},
                {GamePad::Button::DOWN,   (uint8_t) SDLK_DOWN,  -1, -1},
                {GamePad::Button::LEFT,   (uint8_t) SDLK_LEFT,  -1, -1},
                {GamePad::Button::RIGHT,  (uint8_t) SDLK_RIGHT, -1, -1},
        }
};

GamePad::Output *LinuxGamePad::getOutputMode() {
    return &output;
}

void LinuxGamePad::loop() {
    SDL_Event ev;

    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) exit(0);

        switch (ev.type) {
            case SDL_KEYDOWN:
                if (ev.key.keysym.sym == SDLK_UP) m_buttons |= GamePad::Button::UP;
                if (ev.key.keysym.sym == SDLK_DOWN) m_buttons |= GamePad::Button::DOWN;
                if (ev.key.keysym.sym == SDLK_LEFT) m_buttons |= GamePad::Button::LEFT;
                if (ev.key.keysym.sym == SDLK_RIGHT) m_buttons |= GamePad::Button::RIGHT;
                if (ev.key.keysym.sym == SDLK_a) m_buttons |= GamePad::Button::B1;
                if (ev.key.keysym.sym == SDLK_z) m_buttons |= GamePad::Button::B2;
                if (ev.key.keysym.sym == SDLK_e) m_buttons |= GamePad::Button::B3;
                if (ev.key.keysym.sym == SDLK_q) m_buttons |= GamePad::Button::B4;
                if (ev.key.keysym.sym == SDLK_s) m_buttons |= GamePad::Button::B5;
                if (ev.key.keysym.sym == SDLK_d) m_buttons |= GamePad::Button::B6;
                if (ev.key.keysym.sym == SDLK_SPACE) m_buttons |= GamePad::Button::SELECT;
                if (ev.key.keysym.sym == SDLK_RETURN) m_buttons |= GamePad::Button::START;
                break;
            case SDL_KEYUP:
                if (ev.key.keysym.sym == SDLK_UP) m_buttons &= ~GamePad::Button::UP;
                if (ev.key.keysym.sym == SDLK_DOWN) m_buttons &= ~GamePad::Button::DOWN;
                if (ev.key.keysym.sym == SDLK_LEFT) m_buttons &= ~GamePad::Button::LEFT;
                if (ev.key.keysym.sym == SDLK_RIGHT) m_buttons &= ~GamePad::Button::RIGHT;
                if (ev.key.keysym.sym == SDLK_a) m_buttons &= ~GamePad::Button::B1;
                if (ev.key.keysym.sym == SDLK_z) m_buttons &= ~GamePad::Button::B2;
                if (ev.key.keysym.sym == SDLK_e) m_buttons &= ~GamePad::Button::B3;
                if (ev.key.keysym.sym == SDLK_q) m_buttons &= ~GamePad::Button::B4;
                if (ev.key.keysym.sym == SDLK_s) m_buttons &= ~GamePad::Button::B5;
                if (ev.key.keysym.sym == SDLK_d) m_buttons &= ~GamePad::Button::B6;
                if (ev.key.keysym.sym == SDLK_SPACE) m_buttons &= ~GamePad::Button::SELECT;
                if (ev.key.keysym.sym == SDLK_RETURN) m_buttons &= ~GamePad::Button::START;
                break;
            default:
                break;
        }
    }
}
