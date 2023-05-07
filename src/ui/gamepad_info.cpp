//
// Created by cpasjuste on 03/04/23.
//

#include "main.h"
#include "gamepad_info.h"
#include "triangle.h"
#include "rectangle.h"

using namespace uGamePad;

GamePadInfo::GamePadInfo(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap)
        : Bitmap(pos, size, bitmap) {
    m_buttons.push_back({new Circle({70, 32}, 2, true), GamePad::Button::B1});
    m_buttons.push_back({new Circle({78, 24}, 2, true), GamePad::Button::B2});
    m_buttons.push_back({new Circle({62, 24}, 2, true), GamePad::Button::B3});
    m_buttons.push_back({new Circle({70, 16}, 2, true), GamePad::Button::B4});
    m_buttons.push_back({new Circle({17, 3}, 4, true), GamePad::Button::B5});
    m_buttons.push_back({new Circle({71, 3}, 4, true), GamePad::Button::B6});
    m_buttons.push_back({new Rectangle({38, 21}, {4, 7}, true), GamePad::Button::SELECT});
    m_buttons.push_back({new Rectangle({47, 21}, {4, 7}, true), GamePad::Button::START});
    m_buttons.push_back({new Triangle({37, 20}, 6, 180, true), GamePad::Button::UP});
    m_buttons.push_back({new Triangle({37, 32}, 6, 0, true), GamePad::Button::DOWN});
    m_buttons.push_back({new Triangle({31, 26}, 6, 90, true), GamePad::Button::LEFT});
    m_buttons.push_back({new Triangle({43, 26}, 6, -90, true), GamePad::Button::RIGHT});

    // add buttons
    for (const auto &button: m_buttons) {
        button.widget->setVisibility(Visibility::Hidden);
        GamePadInfo::add(button.widget);
    }

    // info text
    auto text = new Text(m_size.x / 2, m_size.y + 2, "SELECT+START:EXIT", Utility::Color::Black);
    text->setOrigin(Origin::Bottom);
    text->setDrawBackground(true);
    GamePadInfo::add(text);
}

void GamePadInfo::loop(const Utility::Vec2i &pos) {
    uint16_t pressed = getPlatform()->getPad()->getButtons();
    for (const auto &button: m_buttons) {
        button.widget->setVisibility(button.button & pressed ? Visibility::Visible : Visibility::Hidden);
    }

    Bitmap::loop(pos);
}
