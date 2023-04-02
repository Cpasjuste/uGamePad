//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_TRIANGLE_H
#define U_GAMEPAD_TRIANGLE_H

#include "widget.h"

namespace uGamePad {
    class Triangle : public Widget {
    public:
        Triangle(const Utility::Vector2i &pos, uint16_t base, uint16_t height,
                 uint8_t rotation = 0, bool fill = false);

        void setPosition(int16_t x, int16_t y) override;

        void setRotation(float rotation);

        void update(const Utility::Vector2i &pos) override;

    private:
        Utility::Vector6f m_triangle{};
        float m_rotation = 0;
        bool m_fill = false;

        static Utility::Vector2f rotate(Utility::Vector2f point, float rotation);

        static Utility::Vector6f calculateTriangle(Utility::Vector2i position, float rotation, float base, float height);
    };
}

#endif //U_GAMEPAD_TRIANGLE_H
