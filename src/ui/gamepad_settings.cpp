//
// Created by cpasjuste on 03/04/23.
//

#include "main.h"
#include "gamepad_settings.h"
#include "triangle.h"
#include "rectangle.h"

using namespace uGamePad;

#define PAD_INFO_TEXT "SELECT+START:EXIT"

GamePadSettings::GamePadSettings(const Utility::Vec2i &pos, const Utility::Vec2i &size, uint8_t *bitmap)
    : Bitmap(pos, size, bitmap) {
    // must match gamepad.h buttons definition
    m_buttons.push_back({new Circle({70, 32}, 2, true), GamePad::Button::B1});
    m_buttons.push_back({new Circle({78, 24}, 2, true), GamePad::Button::B2});
    m_buttons.push_back({new Circle({62, 24}, 2, true), GamePad::Button::B3});
    m_buttons.push_back({new Circle({70, 16}, 2, true), GamePad::Button::B4});
    m_buttons.push_back({new Circle({17, 3}, 4, true), GamePad::Button::B5});
    m_buttons.push_back({new Circle({71, 3}, 4, true), GamePad::Button::B6});
    m_buttons.push_back({new Rectangle({47, 21}, {4, 7}, true), GamePad::Button::START});
    m_buttons.push_back({new Rectangle({38, 21}, {4, 7}, true), GamePad::Button::SELECT});
    // dpad
    m_buttons.push_back({new Triangle({31, 26}, 6, 90, true), GamePad::Button::DPAD_LEFT});
    m_buttons.push_back({new Triangle({43, 26}, 6, -90, true), GamePad::Button::DPAD_RIGHT});
    m_buttons.push_back({new Triangle({37, 20}, 6, 180, true), GamePad::Button::DPAD_UP});
    m_buttons.push_back({new Triangle({37, 32}, 6, 0, true), GamePad::Button::DPAD_DOWN});
    // left axis
    m_buttons.push_back({new Triangle({44, 42}, 6, 90, true), GamePad::Button::AXIS_L_LEFT});
    m_buttons.push_back({new Triangle({63, 42}, 6, -90, true), GamePad::Button::AXIS_L_RIGHT});
    m_buttons.push_back({new Triangle({54, 32}, 6, 180, true), GamePad::Button::AXIS_L_UP});
    m_buttons.push_back({new Triangle({54, 51}, 6, 0, true), GamePad::Button::AXIS_L_DOWN});
    // right axis
    m_buttons.push_back({new Triangle({63, 42}, 6, 90, true), GamePad::Button::AXIS_R_LEFT});
    m_buttons.push_back({new Triangle({82, 42}, 6, -90, true), GamePad::Button::AXIS_R_RIGHT});
    m_buttons.push_back({new Triangle({73, 32}, 6, 180, true), GamePad::Button::AXIS_R_UP});
    m_buttons.push_back({new Triangle({73, 51}, 6, 0, true), GamePad::Button::AXIS_R_DOWN});

    // add buttons
    for (const auto &button: m_buttons) {
        button.widget->setVisibility(Visibility::Hidden);
        GamePadSettings::add(button.widget);
    }

    // info text
    p_text = new Text((int16_t) (m_size.x / 2), (int16_t) (m_size.y + 2),
                      PAD_INFO_TEXT, Utility::Color::Black);
    p_text->setOrigin(Origin::Bottom);
    p_text->setDrawBackground(true);
    GamePadSettings::add(p_text);

    p_newDevice = new Device();
    p_newDevice->report = new InputReportDescriptor();
}

void GamePadSettings::setMode(const Mode &mode) {
    m_mode = mode;
    if (m_mode == Info) {
        p_text->setString(PAD_INFO_TEXT);
        getPlatform()->getPad()->setRepeatDelay(0);
    } else {
        if (!getPlatform()->getPad()->getDevice()) {
            m_mode = Info;
            p_text->setString(PAD_INFO_TEXT);
            getPlatform()->getPad()->setRepeatDelay(0);
            return;
        }

        // restore default device report descriptor to be able to use undefined buttons if any...
        getPlatform()->getPad()->setDeviceDefaultDescriptor();

        // copy default device data/report descriptor to new device
        if (const auto d = getPlatform()->getPad()->getDeviceDefaults()) {
            memcpy(p_newDevice, d, sizeof(Device));
            memcpy(p_newDevice->report, d->report, sizeof(InputReportDescriptor));
        }

        // reset/hide any pressed button
        for (const auto &button: m_buttons) {
            button.widget->setVisibility(Visibility::Hidden);
        }

        getPlatform()->getPad()->setRepeatDelay(UINT16_MAX);

        // clear any previous inputs
        getPlatform()->getPad()->flush();

        // restart
        m_remap_index = 0;
        m_clock_timeout.restart();
    }
}

void GamePadSettings::loop(const Utility::Vec2i &pos) {
    const auto buttons = getPlatform()->getPad()->getButtons();

    if (m_mode == Info) {
        if (buttons & GamePad::Button::MENU ||
            buttons & GamePad::Button::START && buttons & GamePad::Button::SELECT) {
            getPlatform()->getUi()->show(Ui::MenuWidget::MainMenu);
            // needed to clear hardware "menu/start" button
            if (buttons & GamePad::Button::MENU) getPlatform()->getPad()->flush();
            return;
        }
        for (const auto &button: m_buttons) {
            button.widget->setVisibility(button.button & buttons ? Visibility::Visible : Visibility::Hidden);
        }
        Bitmap::loop(pos);
        return;
    }

    if (buttons && !(buttons & GamePad::Button::DELAY) || m_clock_timeout.getElapsedTime().asSeconds() > 5) {
        // remap input descriptor
        if (!(buttons & GamePad::Button::DELAY)) {
            const auto cDesc = getPlatform()->getPad()->getDevice()->report;
            const auto nDesc = p_newDevice->report;
            switch (m_buttons[m_remap_index].button) {
                case GamePad::Button::B1:
                case GamePad::Button::B2:
                case GamePad::Button::B3:
                case GamePad::Button::B4:
                case GamePad::Button::B5:
                case GamePad::Button::B6:
                case GamePad::Button::START:
                case GamePad::Button::SELECT:
                case GamePad::Button::DPAD_LEFT:
                case GamePad::Button::DPAD_RIGHT:
                case GamePad::Button::DPAD_UP:
                case GamePad::Button::DPAD_DOWN:
                    nDesc->joystick.buttons[m_remap_index].byte_offset =
                            cDesc->joystick.buttons[GamePad::getButtonIndex(buttons)].byte_offset;
                    nDesc->joystick.buttons[m_remap_index].bitmask =
                            cDesc->joystick.buttons[GamePad::getButtonIndex(buttons)].bitmask;
                    break;
                case GamePad::Button::AXIS_L_LEFT:
                case GamePad::Button::AXIS_L_UP:
                case GamePad::Button::AXIS_R_LEFT:
                case GamePad::Button::AXIS_R_UP:
#warning "TODO/FIXME: add axis/hat remap support"
                    break;
                default:
                    break;
            }
        }

        m_buttons[m_remap_index].widget->setVisibility(Visibility::Hidden);
        m_remap_index++;

#warning "TODO/FIXME: add axis/hat remap support"
        //if (m_remap_index >= m_buttons.size()) {
        if (m_remap_index >= m_buttons.size() - 8) {
            // all done, update report descriptor of current device
            const auto report = getPlatform()->getPad()->getDevice()->report;
            memcpy(report, p_newDevice->report, sizeof(InputReportDescriptor));
            getPlatform()->getConfig()->saveDevice(getPlatform()->getPad()->getDevice());
            setMode(Info);
            return;
        }

        p_text->setString("PRESS " + Utility::toString(m_buttons[m_remap_index].button));
        m_clock_timeout.restart();
    }

    // update timeout
    p_text->setString("PRESS "
                      + Utility::toString(m_buttons[m_remap_index].button)
                      + "("
                      + std::to_string(5 - (int) m_clock_timeout.getElapsedTime().asSeconds())
                      + ")");

    // update button widget visibility
    if (m_clock_button_visibility.getElapsedTime().asMilliseconds() > 300) {
        m_buttons[m_remap_index].widget->setVisibility(
            m_buttons[m_remap_index].widget->isVisible() ? Visibility::Hidden : Visibility::Visible);
        m_clock_button_visibility.restart();
    }

    Bitmap::loop(pos);
}
