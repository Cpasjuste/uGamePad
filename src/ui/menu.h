//
// Created by cpasjuste on 06/05/23.
//

#ifndef U_GAMEPAD_MENU_H
#define U_GAMEPAD_MENU_H

#include "widget.h"
#include "rectangle.h"
#include "text.h"
#include "menu_line.h"

namespace uGamePad {
    class Menu : public Rectangle {
    public:
        struct MenuOption {
            std::string name;
            std::vector<std::string> values;
            int16_t index = 0;

            std::string value() {
                return values.size() > index ? values[index] : "";
            }

            void next() {
                index++;
                if (index >= values.size()) index = 0;
            }

            void prev() {
                index--;
                if (index < 0) index = (int16_t) (values.size() - 1);
            }
        };

        Menu();

        MenuOption *getSelection();

    private:
        void update();

        void loop(const Utility::Vec2i &pos) override;

        std::vector<MenuOption> m_options;
        std::vector<MenuLine *> p_lines;
        uint8_t max_lines = 0;
        int option_index = 0;
        int highlight_index = 0;
    };
}

#endif //U_GAMEPAD_MENU_H
