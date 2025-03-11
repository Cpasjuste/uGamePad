//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_STARFIELD_BG_H
#define U_GAMEPAD_STARFIELD_BG_H

#include "widget.h"

// TODO: handle position...

namespace uGamePad {
    class StarFieldBg : public Widget {
    public:
        StarFieldBg(int16_t x, int16_t y, int16_t w, int16_t h,
                  const Utility::Color &color = Utility::Color::White);

        StarFieldBg(const Utility::Vec2i &pos, const Utility::Vec2i &size,
                  const Utility::Color &color = Utility::Color::White)
            : StarFieldBg(pos.x, pos.y, size.x, size.y, color) {}

        void loop(const Utility::Vec2i &pos) override;
    };
}

#endif //U_GAMEPAD_STARFIELD_BG_H
