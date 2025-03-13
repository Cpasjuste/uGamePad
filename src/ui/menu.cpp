//
// Created by cpasjuste on 06/05/23.
//

#include "main.h"
#include "menu.h"

using namespace uGamePad;

Menu::Menu() : Rectangle({0, 0}, getPlatform()->getGfx()->getSize()) {
    const uint8_t menu_height = Menu::getSize().y - 4;
    uint8_t line_height = 7 + 2; // font height + margin
    max_lines = menu_height / line_height;
    if (max_lines * line_height < menu_height) {
        line_height = menu_height / max_lines;
    }

    // menu lines
    for (uint8_t i = 0; i < max_lines; i++) {
        auto y = static_cast<int16_t>((line_height * i) + 2);
        auto w = static_cast<int16_t>(Menu::getSize().x - 2);
        auto line = new MenuLine({1, y, w, line_height});
        p_lines.push_back(line);
        Menu::add(line);
    }

    // add option items
    std::vector<std::string> modes;
    for (const auto &out: getPlatform()->getPad()->getOutputModes()) {
        modes.push_back(out.name);
    }
    m_options.push_back({"OUTPUT MODE", modes});
    m_options.push_back({"GAMEPAD TEST", {"Go"}});
    m_options.push_back({"GAMEPAD REMAP", {"Go"}});
    m_options.push_back({"FACTORY RESET", {"Go"}});
    m_options.push_back({"SYSTEM INFO", {"Go"}});
    m_options.push_back({"EXIT", {"Go"}});

    // update lines
    update();
}

void Menu::update() {
    // update lines
    for (uint8_t i = 0; i < max_lines; i++) {
        if (option_index + i >= m_options.size()) {
            p_lines[i]->setVisibility(Visibility::Hidden);
        } else {
            MenuOption *option = &m_options[option_index + i];
            p_lines[i]->setVisibility(Visibility::Visible);
            p_lines[i]->setName(option->name);
            p_lines[i]->setValue(option->value());
            // handle highlight
            p_lines[i]->setHighlighted(i == highlight_index);
        }
    }
}

void Menu::loop(const Utility::Vec2i &pos) {
    if (getPlatform()->getUi()->getMessageBox()->isVisible()) {
        Rectangle::loop(pos);
        return;
    }

    // handle buttons
    const auto buttons = getPlatform()->getPad()->getButtons();
    if (buttons & GamePad::Button::UP) {
        const int index = option_index + highlight_index;
        const int middle = max_lines / 2;
        if (highlight_index <= middle && index - middle > 0) {
            option_index--;
        } else {
            highlight_index--;
        }
        if (highlight_index < 0) {
            highlight_index = (int) m_options.size() < max_lines ? (int) m_options.size() - 1 : max_lines - 1;
            option_index = (int) m_options.size() - 1 - highlight_index;
        }
        update();
    } else if (buttons & GamePad::Button::DOWN) {
        const int index = option_index + highlight_index;
        const int middle = max_lines / 2;
        if (highlight_index >= middle && index + middle < (int) m_options.size()) {
            option_index++;
        } else {
            highlight_index++;
        }
        if (highlight_index >= max_lines || option_index + highlight_index >= (int) m_options.size()) {
            option_index = 0;
            highlight_index = 0;
        }
        update();
    } else if (buttons & GamePad::Button::LEFT) {
        const auto option = getSelection();
        if (option != nullptr) {
            option->prev();
            update();
        }
    } else if (buttons & GamePad::Button::RIGHT) {
        const auto option = getSelection();
        if (option != nullptr) {
            option->next();
            update();
        }
    } else if (buttons & GamePad::Button::B2) {
        getPlatform()->getPad()->flush();
        getPlatform()->getUi()->show(Ui::MenuWidget::Splash);
    } else if (buttons & GamePad::Button::B1 || buttons & GamePad::Button::MENU) {
        const auto option = getSelection();
        if (option != nullptr) {
            if (option->name == "GAMEPAD TEST") {
                getPlatform()->getUi()->show(Ui::MenuWidget::GamePadTest);
            } else if (option->name == "GAMEPAD REMAP") {
                getPlatform()->getUi()->show(Ui::MenuWidget::Remap);
            } else if (option->name == "FACTORY RESET") {
                const auto mbox = getPlatform()->getUi()->getMessageBox();
                mbox->show("FORMAT FLASH ?", "YES", "NO", [mbox](const Text *t) {
                    if (t && t == mbox->getButtonLeft()) {
                        getPlatform()->getFs()->format();
                        Utility::reboot(false);
                    }
                });
            } else if (option->name == "SYSTEM INFO") {
                getPlatform()->getUi()->show(Ui::MenuWidget::InfoMenu);
            } else if (option->name == "EXIT") {
                getPlatform()->getPad()->flush();
                getPlatform()->getUi()->show(Ui::MenuWidget::Splash);
            } else {
                option->next();
                update();
            }

            // needed to clear hardware "menu/start" button
            if (buttons & GamePad::Button::MENU) getPlatform()->getPad()->flush();
        }
    }

    Rectangle::loop(pos);
}

Menu::MenuOption *Menu::getSelection() {
    if (!m_options.empty() && m_options.size() > (size_t) option_index + highlight_index) {
        return &m_options[option_index + highlight_index];
    }
    return nullptr;
}
