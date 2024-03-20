//
// Created by cpasjuste on 03/04/23.
//

#include "main.h"
#include "gamepad_info.h"
#include "triangle.h"
#include "rectangle.h"

using namespace uGamePad;

#define PAD_INFO_TEXT "SELECT+START:EXIT"

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
    // dpad
    m_buttons.push_back({new Triangle({37, 20}, 6, 180, true), GamePad::Button::DPAD_UP});
    m_buttons.push_back({new Triangle({37, 32}, 6, 0, true), GamePad::Button::DPAD_DOWN});
    m_buttons.push_back({new Triangle({31, 26}, 6, 90, true), GamePad::Button::DPAD_LEFT});
    m_buttons.push_back({new Triangle({43, 26}, 6, -90, true), GamePad::Button::DPAD_RIGHT});
    // axis
    m_buttons.push_back({new Triangle({54, 32}, 6, 180, true), GamePad::Button::AXIS_L_UP});
    m_buttons.push_back({new Triangle({54, 51}, 6, 0, true), GamePad::Button::AXIS_L_DOWN});
    m_buttons.push_back({new Triangle({44, 42}, 6, 90, true), GamePad::Button::AXIS_L_LEFT});
    m_buttons.push_back({new Triangle({63, 42}, 6, -90, true), GamePad::Button::AXIS_L_RIGHT});

    // add buttons
    for (const auto &button: m_buttons) {
        button.widget->setVisibility(Visibility::Hidden);
        GamePadInfo::add(button.widget);
    }

    // info text
    auto text = new Text((int16_t) (m_size.x / 2), (int16_t) (m_size.y + 2),
                         PAD_INFO_TEXT, Utility::Color::Black);
    text->setOrigin(Origin::Bottom);
    text->setDrawBackground(true);
    GamePadInfo::add(text);
}

void GamePadInfo::loop(const Utility::Vec2i &pos) {
    uint16_t buttons = getPlatform()->getPad()->getButtons();
    if (buttons & GamePad::Button::START && buttons & GamePad::Button::SELECT) {
        getPlatform()->getUi()->show(Ui::MenuWidget::MainMenu);
        return;
    }

    for (const auto &button: m_buttons) {
        button.widget->setVisibility(button.button & buttons ? Visibility::Visible : Visibility::Hidden);
    }

    Bitmap::loop(pos);
}
