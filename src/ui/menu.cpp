//
// Created by cpasjuste on 06/05/23.
//

#include "main.h"
#include "menu.h"

using namespace uGamePad;

Menu::Menu() : Rectangle({0, 0}, getPlatform()->getGfx()->getSize()) {
    uint8_t menu_height = Menu::getSize().y - 4;
    uint8_t line_height = 7 + 2; // font height + margin
    max_lines = menu_height / line_height;
    if (max_lines * line_height < menu_height) {
        line_height = menu_height / max_lines;
    }

    for (uint8_t i = 0; i < max_lines; i++) {
        auto y = (int16_t) ((line_height * i) + 4);
        auto line = new Text(3, y, "OPTION " + std::to_string(i));
        m_lines.push_back(line);
        Menu::add(line);
    }
}

void Menu::update(const Utility::Vector2i &pos) {
    for (uint8_t i = 0; i < max_lines; i++) {
        /*
        if (start_index + i >= files.size()) {
            lines[i]->setVisibility(Visibility::Hidden);
        } else {

        }
        */
        // handle highlight
        if (i == highlight_index) {
            m_lines[i]->setColor(Utility::Color::Black);
            m_lines[i]->setDrawBackground(true);
        }
    }

    Rectangle::update(pos);
}
