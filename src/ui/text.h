//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_TEXT_H
#define U_GAMEPAD_TEXT_H

#include "widget.h"

namespace uGamePad {
    class Text : public Widget {
    public:
        Text(int16_t x, int16_t y, const std::string &text);

        Text(const Utility::Vector2i &pos, const std::string &text);

        void setString(const std::string &str);

        void update(const Utility::Vector2i &pos) override;

    private:
        std::string m_text;
    };
}

#endif //U_GAMEPAD_TEXT_H
