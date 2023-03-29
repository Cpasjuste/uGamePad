//
// Created by cpasjuste on 28/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include <cstdint>
#include "Adafruit_GFX.h"

namespace uGamePad {
    class Ui {
    public:
        enum Color {
            Black = 0,
            White = 1,
            Inverse = 2
        };

        struct Vector2 {
            int16_t x;
            int16_t y;
        };

        explicit Ui(Vector2 size = {128, 64});

        virtual void flip() {};

        virtual uint16_t getColor(Color color);

        void drawSplash();

    protected:
        Adafruit_GFX *p_display = nullptr;
        Vector2 m_size = {128, 64};
    };
}

#endif //U_GAMEPAD_UI_H
