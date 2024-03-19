//
// Created by cpasjuste on 18/03/24.
//

#include "main.h"
#include "remap.h"

using namespace uGamePad;

UiRemap::UiRemap(const uGamePad::Utility::Vec2i &pos, const uGamePad::Utility::Vec2i &size) : Widget() {
    UiRemap::setPosition(pos);
    UiRemap::setSize(size);

    p_text = new Text(UiRemap::getSize().x / 2, UiRemap::getSize().y / 2,
                      "          PRESS ANY\n   BUTTON TO START");
    p_text->setOrigin(Origin::Center);
    UiRemap::add(p_text);

    p_newDevice = new Device();
    p_newDevice->report = new InputReportDescriptor();
}

void UiRemap::setVisibility(Widget::Visibility visibility) {
    if (visibility == Visibility::Visible) {
        // clear inputs
        while (true) {
            if (!getPlatform()->getPad()->getButtons()) {
                break;
            }
            getPlatform()->loop();
        }

        // copy current device data to temp/remapped device
        memcpy(p_newDevice, getPlatform()->getPad()->getDevice(), sizeof(Device));
        memcpy(p_newDevice->report, getPlatform()->getPad()->getDevice()->report, sizeof(InputReportDescriptor));

        // reset
        m_started = false;
        m_button_index = 0;
    }

    Widget::setVisibility(visibility);
}

void UiRemap::loop(const Utility::Vec2i &pos) {
    if (m_visibility == Visibility::Hidden) return;

    uint16_t buttons = getPlatform()->getPad()->getButtons();

    if (!m_started && buttons && !(buttons & GamePad::Button::DELAY)) {
        m_started = true;
        p_text->setString("PRESS BTN (1)");
    } else if (m_started && buttons && !(buttons & GamePad::Button::DELAY)) {
        printf("BUTTONS: %s\r\n", Utility::toString(buttons).c_str());
        // TODO: cleanup this crap...
        auto currDesc = getPlatform()->getPad()->getDevice()->report;
        auto newDesc = p_newDevice->report;
        switch (buttons) {
            case GamePad::Button::B1:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[0].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[0].bitmask;
                break;
            case GamePad::Button::B2:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[1].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[1].bitmask;
                break;
            case GamePad::Button::B3:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[2].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[2].bitmask;
                break;
            case GamePad::Button::B4:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[3].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[3].bitmask;
                break;
            case GamePad::Button::B5:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[4].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[4].bitmask;
                break;
            case GamePad::Button::B6:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[5].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[5].bitmask;
                break;
            case GamePad::Button::START:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[6].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[6].bitmask;
                break;
            case GamePad::Button::SELECT:
                newDesc->joystick.buttons[m_button_index].byte_offset = currDesc->joystick.buttons[7].byte_offset;
                newDesc->joystick.buttons[m_button_index].bitmask = currDesc->joystick.buttons[7].bitmask;
                break;
            default:
                return;
        }

        m_button_index++;
        if (m_button_index >= 8) {
            m_started = false;
            m_button_index = 0;
            p_text->setString("ALL DONE");
            return;
        }

        std::string text = "PRESS BTN (";
        switch (m_button_index) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                text += std::to_string(m_button_index + 1) + ")";
                break;
            case 6:
                text += "START)";
                break;
            case 7:
                text += "SELECT)";
                break;
            default:
                break;
        }

        if (p_text->getString() != text) {
            p_text->setString(text);
        }
    }

    Widget::loop(pos);
}
