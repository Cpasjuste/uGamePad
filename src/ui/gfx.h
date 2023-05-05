//
// Created by cpasjuste on 28/03/23.
//

#ifndef U_GAMEPAD_GFX_H
#define U_GAMEPAD_GFX_H

#include <cstdint>
#include "Adafruit_GFX.h"
#include "utility/utility.h"

namespace uGamePad {
    class Gfx {
    public:
        explicit Gfx(Utility::Vector2i size = {128, 64});

        virtual Utility::Vector2i getSize();

        virtual void clear();

        virtual void flip() {};

        Adafruit_GFX *getDisplay() { return p_display; };

    protected:
        Adafruit_GFX *p_display = nullptr;
        Utility::Vector2i m_size = {128, 64};
    };
}

#endif //U_GAMEPAD_GFX_H
