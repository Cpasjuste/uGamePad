//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "pico_gamepad.h"
#include "devices.h"
#include "utility.h"

using namespace uGamePad;

// RP2040-Zero pinout
static GamePad::PinMapping pinMapping[MAX_BUTTONS] = {
#if ARDUINO_SEEED_XIAO_RP2040
        {GamePad::Button::B1, D0},
        {GamePad::Button::B2, D1},
        {GamePad::Button::B3, D2},
        {GamePad::Button::B4, D3},
        {GamePad::Button::B5, D4},
        {GamePad::Button::B6, D5},
        {GamePad::Button::SELECT, D8},
        {GamePad::Button::START, D9},
        {GamePad::Button::UP, D10},
        {GamePad::Button::DOWN, 0},
        {GamePad::Button::LEFT, 0},
        {GamePad::Button::RIGHT, 0},
#else
        {GamePad::Button::B1,     D9},
        {GamePad::Button::B2,     D10},
        {GamePad::Button::B3,     D11},
        {GamePad::Button::B4,     D12},
        {GamePad::Button::B5,     D3},
        {GamePad::Button::B6,     D8},
        {GamePad::Button::SELECT, D7},
        {GamePad::Button::START,  D15},
        {GamePad::Button::UP,     D26},
        {GamePad::Button::DOWN,   D27},
        {GamePad::Button::LEFT,   D28},
        {GamePad::Button::RIGHT,  D29},
#endif
};

bool PicoGamePad::update(const uint8_t *report, uint16_t len) {
    if (!p_device || !p_device->report) {
        TU_LOG1("uGamePad::loop: error: device not set\r\n");
        return false;
    }

    TU_LOG2("uGamePad::loop: received report for '%s': type: %i, len: %i)\r\n",
            p_device->name, p_device->type, len);

    // do not process bytes if less than x bytes
    if (len < p_device->report->min_bytes) return true;

    // reset buttons state
    m_buttons = 0;

    // process buttons
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (p_device->report->buttons[i].byte_index >= len) continue;
        m_buttons |= report[p_device->report->buttons[i].byte_index] &
                     p_device->report->buttons[i].button_index ? (1 << i) : 0;
    }

    // process axis
    for (int i = 0; i < 3; i += 2) {
        if (p_device->report->axis[i].byte_index >= len) continue;
        if (p_device->report->axis[i].type == AxisType::AXIS_32767) {
            int16_t x = (int16_t &) report[p_device->report->axis[i].byte_index];
            int16_t y = (int16_t &) report[p_device->report->axis[i + 1].byte_index];
            m_buttons |= GamePad::getButtonsFromAxis(x, y);
        }
    }

    // process hat
    if (p_device->report->hat.byte_index < len) {
        m_buttons |= GamePad::getButtonsFromHat(report[p_device->report->hat.byte_index]);
    }

    if (m_buttons != 0) TU_LOG1("%s: %s\r\n", p_device->name, Utility::toString(m_buttons).c_str());

    /*
    if (p_device->type == TYPE_XBOX360) {
        // xbox sends a 3 byte report that we don't care for now
        // (todo: look what this packet is)
        if (len == 3) return true;

        m_buttons = 0;
        for (int i = 0; i < MAX_BUTTONS; i++) {
            if (XBOXDevice::buttons[i].byte_index >= len) continue;
            m_buttons |= report[XBOXDevice::buttons[i].byte_index] &
                         XBOXDevice::buttons[i].button_index ? (1 << i) : 0;
        }
        if (XBOXDevice::hat.byte_index < len) {
            m_buttons |= GamePad::getButtonsFromHat(report[XBOXDevice::hat.byte_index]);
        }

        for (int i = 0; i < 3; i += 2) {
            if (XBOXDevice::axis[i].byte_index >= len) continue;
            if (XBOXDevice::axis[i].type == AxisType::AXIS_32767) {
                int16_t x = (int16_t &) report[XBOXDevice::axis[i].byte_index];
                int16_t y = (int16_t &) report[XBOXDevice::axis[i + 1].byte_index];
                m_buttons |= GamePad::getButtonsFromAxis(x, y);
            }
        }
        if (m_buttons != 0) TU_LOG1("xbox: %s\r\n", Utility::toString(m_buttons).c_str());
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
            TU_LOG2("uGamePad::loop: skipping report, wrong packet (ds4)\r\n");
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
    } else if (p_device->type == TYPE_NEOGEO_MINI) {
        if (len == 27) {
            m_buttons = 0;
            for (int i = 0; i < MAX_BUTTONS; i++) {
                if (NGMDevice::buttons[i].byte_index >= len) continue;
                m_buttons |= report[NGMDevice::buttons[i].byte_index] &
                             NGMDevice::buttons[i].button_index ? (1 << i) : 0;
            }
            if (NGMDevice::hat.byte_index < len) {
                m_buttons |= GamePad::getButtonsFromHat(report[NGMDevice::hat.byte_index]);
            }
            if (m_buttons != 0) TU_LOG1("ng-mini: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("tuh_hid_report_received_cb: skipping report, wrong packet (ng-mini)\r\n");
            return false;
        }
    } else {
        return false;
    }
    */

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
