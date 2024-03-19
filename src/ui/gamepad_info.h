//
// Created by cpasjuste on 03/04/23.
//

#ifndef U_GAMEPAD_GAMEPAD_INFO_H
#define U_GAMEPAD_GAMEPAD_INFO_H

#include "circle.h"
#include "bitmap.h"
#include "text.h"

namespace uGamePad {
    class GamePadInfo : public Bitmap {
    public:
        GamePadInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap);

        void loop(const Utility::Vec2i &pos) override;

    private:
        struct GamePadButton {
            Widget *widget = nullptr;
            uint16_t button{};
        };

        std::vector<GamePadButton> m_buttons;
    };
}

#endif //U_GAMEPAD_GAMEPAD_INFO_H
