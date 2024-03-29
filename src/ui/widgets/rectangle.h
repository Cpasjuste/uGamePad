//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_RECTANGLE_H
#define U_GAMEPAD_RECTANGLE_H

#include "widget.h"

namespace uGamePad {
    class Rectangle : public Widget {
    public:
        Rectangle(int16_t x, int16_t y, int16_t w, int16_t h,
                  bool fill = false, int16_t radius = 0,
                  const Utility::Color &color = Utility::Color::White);

        Rectangle(const Utility::Vec2i &pos, const Utility::Vec2i &size,
                  bool fill = false, int16_t radius = 0,
                  const Utility::Color &color = Utility::Color::White);

        void loop(const Utility::Vec2i &pos) override;

    private:
        int16_t m_radius = 0;
        bool m_fill = false;
    };
}

#endif //U_GAMEPAD_RECTANGLE_H
