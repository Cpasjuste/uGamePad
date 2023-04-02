//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_LINE_H
#define U_GAMEPAD_LINE_H

#include "widget.h"

namespace uGamePad {
    class Line : public Widget {
    public:
        Line(const Utility::Vector2i &pos, int16_t size, float rotation = 0);

        void update(const Utility::Vector2i &pos) override;

    private:
        Utility::Line m_line{};
        float m_rotation;

        static Utility::Line calculateLine(Utility::Vector2f position, float rotation, float length);
    };
}

#endif //U_GAMEPAD_LINE_H
