//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_LINE_H
#define U_GAMEPAD_LINE_H

#include "widget.h"

namespace uGamePad {
    class Line : public Widget {
    public:
        Line(const Utility::Vector2i &pos, int16_t length, float rotation = 0);

        void update(const Utility::Vector2i &pos) override;

    private:
        Utility::Line m_line{};
        float m_rotation;

        Utility::Line calculateLine(Utility::Vector2f position, float length, float rotation);
    };
}

#endif //U_GAMEPAD_LINE_H
