//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_TEXT_H
#define U_GAMEPAD_TEXT_H

#include "widget.h"
#include "rectangle.h"

namespace uGamePad {
    class Text : public Widget {
    public:
        Text(int16_t x, int16_t y, const std::string &text,
             const Utility::Color &color = Utility::Color::White);

        Text(const Utility::Vector2i &pos, const std::string &text,
             const Utility::Color &color = Utility::Color::White);

        void setString(const std::string &str);

        void setDrawBackground(bool drawBg);

        void loop(const Utility::Vector2i &pos) override;

    private:
        std::string m_text;
        Rectangle *m_bg_rect = nullptr;
    };
}

#endif //U_GAMEPAD_TEXT_H
