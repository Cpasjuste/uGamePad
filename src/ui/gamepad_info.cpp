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
        GamePadInfo::add(button.widget);
    }

    // info text
    p_text = new Text((int16_t) (m_size.x / 2), (int16_t) (m_size.y + 2),
                      PAD_INFO_TEXT, Utility::Color::Black);
    p_text->setOrigin(Origin::Bottom);
    p_text->setDrawBackground(true);
    GamePadInfo::add(p_text);

    p_newDevice = new Device();
    p_newDevice->report = new InputReportDescriptor();
}

void GamePadInfo::setMode(const Mode &mode) {
    m_mode = mode;
    if (m_mode == Mode::Info) {
        p_text->setString(PAD_INFO_TEXT);
        getPlatform()->getPad()->setRepeatDelay(0);
    } else {
        // copy current device data to temp/remapped device
        const auto device = getPlatform()->getPad()->getDevice();
        if (!device) {
            m_mode = Mode::Info;
            p_text->setString(PAD_INFO_TEXT);
            return;
        }

        // copy actual device data to new device
        // save "new device" report ptr
        void *report = p_newDevice->report;
        memcpy(p_newDevice, device, sizeof(Device));
        // restore report ptr
        p_newDevice->report = static_cast<InputReportDescriptor *>(report);
        memcpy(p_newDevice->report, device->report, sizeof(InputReportDescriptor));

        // clear inputs before starting
        getPlatform()->getPad()->flush();
        getPlatform()->getPad()->setRepeatDelay(UINT16_MAX);

        // restart
        m_clock_timeout.restart();
        m_button_index = 0;
    }
}

void GamePadInfo::loop(const Utility::Vec2i &pos) {
    if (!getPlatform()->getPad()->getDevice()) {
        Bitmap::loop(pos);
        return;
    }

    const auto buttons = getPlatform()->getPad()->getButtons();
    if (m_mode == Mode::Info) {
        if (buttons & GamePad::Button::START && buttons & GamePad::Button::SELECT) {
            getPlatform()->getUi()->show(Ui::MenuWidget::MainMenu);
            return;
        }
        for (const auto &button: m_buttons) {
            button.widget->setVisibility(button.button & buttons ? Visibility::Visible : Visibility::Hidden);
        }
    } else {
        if (buttons && !(buttons & GamePad::Button::DELAY) || m_clock_timeout.getElapsedTime().asSeconds() > 5) {
            // remap input descriptor
            if (!(buttons & GamePad::Button::DELAY)) {
#warning "TODO: finish this (axis/hat)"
                const auto cDesc = getPlatform()->getPad()->getDevice()->report;
                const auto nDesc = p_newDevice->report;
                switch (buttons) {
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
                        nDesc->joystick.buttons[m_button_index].byte_offset =
                                cDesc->joystick.buttons[GamePad::getButtonIndex(buttons)].byte_offset;
                        nDesc->joystick.buttons[m_button_index].bitmask =
                                cDesc->joystick.buttons[GamePad::getButtonIndex(buttons)].bitmask;
                        break;
                    default:
                        return;
                }
            }

            m_buttons[m_button_index].widget->setVisibility(Widget::Visibility::Hidden);
            m_button_index++;
            if (m_button_index >= m_buttons.size()) {
                // all done
                const auto report = getPlatform()->getPad()->getDevice()->report;
                memcpy(report, p_newDevice->report, sizeof(InputReportDescriptor));
                getPlatform()->getConfig()->saveDevice(getPlatform()->getPad()->getDevice());
                setMode(Info);
                return;
            }

            p_text->setString("PRESS " + Utility::toString(m_buttons[m_button_index].button));
            m_clock_timeout.restart();
        }

        // update timeout
        p_text->setString("PRESS "
                          + Utility::toString(m_buttons[m_button_index].button)
                          + "("
                          + std::to_string(5 - (int) m_clock_timeout.getElapsedTime().asSeconds())
                          + ")");

        // update button widget visibility
        if (m_clock_button_visibility.getElapsedTime().asMilliseconds() > 300) {
            m_buttons[m_button_index].widget->setVisibility(
                m_buttons[m_button_index].widget->isVisible() ? Visibility::Hidden : Visibility::Visible);
            m_clock_button_visibility.restart();
        }
    }

    Bitmap::loop(pos);
}
