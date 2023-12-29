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

    // lines
    for (uint8_t i = 0; i < max_lines; i++) {
        auto y = (int16_t)((line_height * i) + 2);
        auto w = (int16_t)(Menu::getSize().x - 2);
        auto line = new MenuLine({1, y, w, line_height});
        p_lines.push_back(line);
        Menu::add(line);
    }

    // add option items
    std::vector <std::string> modes;
    for (const auto &out: getPlatform()->getPad()->getOutputModes()) {
        modes.push_back(out.name);
    }
    m_options.push_back({"OUTPUT MODE", modes});
#ifdef WIP_DISABLE_OPTION_REMAP
    m_options.push_back({"BUTTONS REMAP", {"TODO"}});
#else
    m_options.push_back({"BUTTONS REMAP", {"TODO"}});
#endif
#ifdef WIP_DISABLE_OPTION_AUTO_FIRE
    m_options.push_back({"AUTO FIRE", {"TODO"}});
#else
    m_options.push_back({"AUTO FIRE", {"OFF", "ON"}});
#endif
    m_options.push_back({"GAMEPAD TEST", {"GO"}});
    m_options.push_back({"EXIT", {"GO"}});

    // update lines
    update();
}

void Menu::update() {
// update lines
    for (uint8_t i = 0; i < max_lines; i++) {
        if (start_index + i >= m_options.size()) {
            p_lines[i]->setVisibility(Visibility::Hidden);
        } else {
            MenuOption *option = &m_options[start_index + i];
            p_lines[i]->setVisibility(Visibility::Visible);
            p_lines[i]->setName(option->name);
            p_lines[i]->setValue(option->value());
            // handle highlight
            p_lines[i]->setHighlighted(i == highlight_index);
        }
    }
}

void Menu::loop(const Utility::Vec2i &pos) {
    // handle buttons
    uint16_t buttons = getPlatform()->getPad()->getButtons();
    if (buttons & GamePad::Button::UP) {
        option_index--;
        if (option_index < 0) {
            option_index = (int) m_options.size() - 1;
            start_index = (int) m_options.size() - max_lines;
            highlight_index = max_lines - 1;
        } else {
            if (highlight_index <= 0) {
                start_index--;
            } else {
                highlight_index--;
            }
        }
        update();
    } else if (buttons & GamePad::Button::DOWN) {
        option_index++;
        if (option_index >= m_options.size()) {
            option_index = 0;
            start_index = 0;
            highlight_index = 0;
        } else {
            if (highlight_index >= max_lines - 1) {
                start_index++;
            } else {
                highlight_index++;
            }
        }
        update();
    } else if (buttons & GamePad::Button::LEFT) {
        m_options[option_index].prev();
        update();
    } else if (buttons & GamePad::Button::RIGHT) {
        m_options[option_index].next();
        update();
    } else if (buttons & GamePad::Button::B1 || buttons & GamePad::Button::START) {
        if (m_options[option_index].name == "GAMEPAD TEST") {
            getPlatform()->getUi()->show(Ui::MenuWidget::GamePadTest);
        } else if (m_options[option_index].name == "EXIT") {
            getPlatform()->getUi()->show(Ui::MenuWidget::Splash);
        } else {
            m_options[option_index].next();
            update();
        }
    }

    Rectangle::loop(pos);
}
