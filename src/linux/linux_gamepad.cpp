//
// Created by cpasjuste on 24/03/23.
//

#include <SDL2/SDL.h>
#include "linux_gamepad.h"

using namespace uGamePad;

bool LinuxGamePad::update(const uint8_t *report, uint16_t len) {
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

    return true;
}
