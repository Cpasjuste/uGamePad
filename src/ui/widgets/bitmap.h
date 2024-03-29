//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_BITMAP_H
#define U_GAMEPAD_BITMAP_H

#include "widget.h"

namespace uGamePad {
    class Bitmap : public Widget {
    public:
        Bitmap(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap);

        void loop(const Utility::Vec2i &pos) override;

    private:
        uint8_t *m_bitmap = nullptr;
    };
}

#endif //U_GAMEPAD_BITMAP_H
