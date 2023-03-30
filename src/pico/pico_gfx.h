//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_GFX_H
#define U_GAMEPAD_PICO_GFX_H

#include "gfx.h"

namespace uGamePad {
    class PicoGfx : public Gfx {
    public:
        PicoGfx();

        void flip() override;
    };
}

#endif //U_GAMEPAD_PICO_GFX_H
