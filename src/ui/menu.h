//
// Created by cpasjuste on 06/05/23.
//

#ifndef U_GAMEPAD_MENU_H
#define U_GAMEPAD_MENU_H

#include "widget.h"
#include "rectangle.h"
#include "text.h"

namespace uGamePad {
    class Menu : public Rectangle {
    public:
        Menu();

    private:
        void update(const Utility::Vector2i &pos) override;

        std::vector<Text *> m_lines;
        uint8_t max_lines = 0;
        uint8_t start_index = 0;
        uint8_t option_index = 0;
        uint8_t highlight_index = 0;
    };
}

#endif //U_GAMEPAD_MENU_H
