//
// Created by cpasjuste on 03/04/23.
//

#ifndef U_GAMEPAD_GAMEPAD_SETTINGS_H
#define U_GAMEPAD_GAMEPAD_SETTINGS_H

#include "circle.h"
#include "bitmap.h"
#include "text.h"

namespace uGamePad {
    class GamePadSettings : public Bitmap {
    public:
        enum Mode {
            Info,
            Remap
        };

        GamePadSettings(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap);

        void loop(const Utility::Vec2i &pos) override;

        void setMode(const Mode &mode);

    private:
        struct GamePadButton {
            Widget *widget = nullptr;
            uint32_t button{};
        };

        Text *p_text;
        Mode m_mode = Info;
        Device *p_newDevice;
        uint8_t m_button_index = 0;
        std::vector<GamePadButton> m_buttons;
        Clock m_clock_button_visibility, m_clock_timeout;
    };
}

#endif //U_GAMEPAD_GAMEPAD_SETTINGS_H
