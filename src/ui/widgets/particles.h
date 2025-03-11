//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_PARTICLES_H
#define U_GAMEPAD_PARTICLES_H

#include "widget.h"

// TODO: handle position...

namespace uGamePad {
    class ParticleBg : public Widget {
    public:
        ParticleBg(int16_t x, int16_t y, int16_t w, int16_t h,
                   const Utility::Color &color = Utility::Color::White);

        ParticleBg(const Utility::Vec2i &pos, const Utility::Vec2i &size,
                   const Utility::Color &color = Utility::Color::White)
            : ParticleBg(pos.x, pos.y, size.x, size.y, color) {
        }

        void loop(const Utility::Vec2i &pos) override;
    };
}

#endif //U_GAMEPAD_PARTICLES_H
