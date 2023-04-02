//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_TRIANGLE_H
#define U_GAMEPAD_TRIANGLE_H

#include "widget.h"

namespace uGamePad {
    class Triangle : public Widget {
    public:
        Triangle(const Utility::Vector2i &pos, uint16_t size, float rotation = 0, bool fill = false);

        void setPosition(int16_t x, int16_t y) override;

        void setRotation(float rotation);

        void update(const Utility::Vector2i &pos) override;

    private:
        Utility::Vector6f m_triangle{};
        float m_rotation = 0;
        bool m_fill = false;

        Utility::Vector6f calculateTriangle(Utility::Vector2i position, float size, float rotation);
    };
}

#endif //U_GAMEPAD_TRIANGLE_H
