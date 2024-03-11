//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"
#include "linux_display.h"

using namespace uGamePad;

LinuxDisplay::LinuxDisplay() : Adafruit_GFX(128, 64) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s", SDL_GetError());
        return;
    }

    p_window = SDL_CreateWindow("uGamePad", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                width(), height(), SDL_WINDOW_SHOWN);
    if (!p_window) {
        fprintf(stderr, "LinuxDisplay: SDL_CreateWindow error: %s", SDL_GetError());
        return;
    }

    p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!p_renderer) {
        fprintf(stderr, "LinuxDisplay: SDL_CreateRenderer error: %s", SDL_GetError());
        return;
    }

    printf("LinuxDisplay: %i x %i\r\n", width(), height());
}

void LinuxDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (color) {
        SDL_SetRenderDrawColor(p_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    } else {
        SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }

    SDL_RenderDrawPoint(p_renderer, x, y);
}

void LinuxDisplay::clear() {
    SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(p_renderer);
}

void LinuxDisplay::flip() {
    SDL_RenderPresent(p_renderer);
}
