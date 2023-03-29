//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "pico_gamepad.h"
#include "utility.h"

using uGamePad::GamePad;
using uGamePad::PicoGamePad;

static GamePad::PinMapping pinMapping[MAX_BUTTONS] = {
        {GamePad::Button::B1,     D2},
        {GamePad::Button::B2,     D1},
        {GamePad::Button::B3,     D0},
        {GamePad::Button::B4,     D10},
        {GamePad::Button::B5,     D8},
        {GamePad::Button::B6,     D7},
        {GamePad::Button::SELECT, D11},
        {GamePad::Button::START,  D9},
        {GamePad::Button::UP,     D5},
        {GamePad::Button::DOWN,   D6},
        {GamePad::Button::LEFT,   D4},
        {GamePad::Button::RIGHT,  D3},
};

bool PicoGamePad::update(const uint8_t *report, uint16_t len) {
    if (!p_device) {
        TU_LOG1("uGamePad::update: error: device not set\r\n");
        return false;
    }

    TU_LOG2("uGamePad::update: received report for '%s': type: %i, len: %i)\r\n",
            p_device->name, p_device->type, len);

    if (p_device->type == TYPE_XBOX360) {
        if (sizeof(XBOXReport) == len && report[0] == 0x00) {
            auto r = reinterpret_cast<const XBOXReport *>(report);
            m_buttons =
                    (r->buttons & XBOXReport::Button::A ? GamePad::Button::B1 : 0) |
                    (r->buttons & XBOXReport::Button::B ? GamePad::Button::B2 : 0) |
                    (r->buttons & XBOXReport::Button::X ? GamePad::Button::B3 : 0) |
                    (r->buttons & XBOXReport::Button::Y ? GamePad::Button::B4 : 0) |
                    (r->buttons & XBOXReport::Button::LB ? GamePad::Button::B5 : 0) |
                    (r->buttons & XBOXReport::Button::RB ? GamePad::Button::B6 : 0) |
                    (r->buttons & XBOXReport::Button::PAD_UP ? GamePad::Button::UP : 0) |
                    (r->buttons & XBOXReport::Button::PAD_DOWN ? GamePad::Button::DOWN : 0) |
                    (r->buttons & XBOXReport::Button::PAD_LEFT ? GamePad::Button::LEFT : 0) |
                    (r->buttons & XBOXReport::Button::PAD_RIGHT ? GamePad::Button::RIGHT : 0) |
                    (r->buttons & XBOXReport::Button::BACK ? GamePad::Button::SELECT : 0) |
                    (r->buttons & XBOXReport::Button::START ? GamePad::Button::START : 0);
            m_buttons |= GamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1]);
            m_buttons |= GamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1]);
            if (m_buttons != 0) TU_LOG1("x360: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("uGamePad::update: skipping report, wrong packet (xbox)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS4) {
        if (sizeof(DS4Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS4Report *>(report);
            m_buttons =
                    (r->buttons1 & DS4Report::Button1::CROSS ? GamePad::Button::B1 : 0) |
                    (r->buttons1 & DS4Report::Button1::CIRCLE ? GamePad::Button::B2 : 0) |
                    (r->buttons1 & DS4Report::Button1::SQUARE ? GamePad::Button::B3 : 0) |
                    (r->buttons1 & DS4Report::Button1::TRIANGLE ? GamePad::Button::B4 : 0) |
                    (r->buttons2 & DS4Report::Button2::L1 ? GamePad::Button::B5 : 0) |
                    (r->buttons2 & DS4Report::Button2::R1 ? GamePad::Button::B6 : 0) |
                    (r->buttons2 & DS4Report::Button2::SHARE ? GamePad::Button::SELECT : 0) |
                    (r->t_pad ? GamePad::Button::SELECT : 0) |
                    (r->buttons2 & DS4Report::Button2::OPTIONS ? GamePad::Button::START : 0);
            // axis
            m_buttons |= GamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1], AXIS_255 | AXIS_FLIP_Y);
            m_buttons |= GamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1], AXIS_255 | AXIS_FLIP_Y);
            // hat
            m_buttons |= GamePad::getButtonsFromHat(r->buttons1 & 15);
            if (m_buttons != 0) TU_LOG1("ds4: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("uGamePad::update: skipping report, wrong packet (ds4)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS5) {
        if (sizeof(DS5Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS5Report *>(report);
            auto buttons = r->buttons[0] | (r->buttons[1] << 8) | (r->buttons[2] << 16);
            m_buttons =
                    (buttons & DS4Report::Button1::CROSS ? GamePad::Button::B1 : 0) |
                    (buttons & DS4Report::Button1::CIRCLE ? GamePad::Button::B2 : 0) |
                    (buttons & DS4Report::Button1::TRIANGLE ? GamePad::Button::B3 : 0) |
                    (buttons & DS4Report::Button1::SQUARE ? GamePad::Button::B4 : 0) |
                    (buttons & DS4Report::Button2::L1 ? GamePad::Button::B5 : 0) |
                    (buttons & DS4Report::Button2::R1 ? GamePad::Button::B6 : 0) |
                    (buttons & DS4Report::Button2::SHARE ? GamePad::Button::SELECT : 0) |
                    (buttons & DS4Report::Button2::OPTIONS ? GamePad::Button::START : 0);
            // axis
            m_buttons |= GamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1], AXIS_255 | AXIS_FLIP_Y);
            m_buttons |= GamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1], AXIS_255 | AXIS_FLIP_Y);
            // hat
            m_buttons |= GamePad::getButtonsFromHat(r->buttons[0] & 15);
            if (m_buttons != 0) TU_LOG1("ds5: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("tuh_hid_report_received_cb: skipping report, wrong packet (ds5)\r\n");
            return false;
        }
    } else {
        return false;
    }

    return true;
}

GamePad::PinMapping *PicoGamePad::getPinMapping() {
    return pinMapping;
}

void PicoGamePad::setLed(uint8_t type) {
    // TODO: handle type, ds4, etc...
    if (p_device && p_device->type == TYPE_XBOX360) {
        uint8_t msg[3] = {0x01, 0x03, 0x02};
        tuh_hid_set_report(m_addr, m_instance, 5, HID_REPORT_TYPE_OUTPUT, &msg, 3);
    }
}
