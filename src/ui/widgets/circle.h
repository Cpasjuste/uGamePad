//
// Created by cpasjuste on 03/04/23.
//

#ifndef U_GAMEPAD_CIRCLE_H
#define U_GAMEPAD_CIRCLE_H

#include "widget.h"

namespace uGamePad {
    class Circle : public Widget {
    public:
        Circle(const Utility::Vector2i &pos, int16_t size, bool fill = false);

        void update(const Utility::Vector2i &pos) override;

    private:
        bool m_fill = false;
    };
}

#endif //U_GAMEPAD_CIRCLE_H
