//
// Created by cpasjuste on 18/03/24.
//

#ifndef U_GAMEPAD_REMAP_H
#define U_GAMEPAD_REMAP_H

#include "widget.h"
#include "text.h"

namespace uGamePad {
    class UiRemap : public Widget {
    public:
        UiRemap(const Utility::Vec2i &pos, const Utility::Vec2i &size);

        void loop(const Utility::Vec2i &pos) override;

        void setVisibility(Widget::Visibility visibility) override;

    private:
        Text *p_text;
        Device *p_newDevice;
        bool m_started = false;
        uint8_t m_button_index = 0;
    };
}

#endif //U_GAMEPAD_REMAP_H
