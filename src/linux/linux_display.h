//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_DISPLAY
#define U_GAMEPAD_LINUX_DISPLAY

#include <SDL2/SDL.h>
#include "Arduino.h"
#include "Adafruit_GFX.h"

namespace uGamePad {
    class LinuxDisplay : public Adafruit_GFX {
    public:
        LinuxDisplay();

        void drawPixel(int16_t x, int16_t y, uint16_t color) override;

        void flip();

        void clear();

    private:
        SDL_Window *p_window = nullptr;
        SDL_Renderer *p_renderer = nullptr;
    };
}

#endif // U_GAMEPAD_LINUX_DISPLAY
