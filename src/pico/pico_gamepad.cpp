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
    if (len < p_device->report->min_size) return true;

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
        if (p_device->report->axis[i].type & Report::AxisType::AXIS_I16) {
            int16_t x = (int16_t &) report[p_device->report->axis[i].byte_index];
            int16_t y = (int16_t &) report[p_device->report->axis[i + 1].byte_index];
            m_buttons |= GamePad::getButtonsFromAxis(x, y, p_device->report->axis[i].type);
        } else if (p_device->report->axis[i].type & Report::AxisType::AXIS_UI8) {
            uint8_t x = (uint8_t &) report[p_device->report->axis[i].byte_index];
            uint8_t y = (uint8_t &) report[p_device->report->axis[i + 1].byte_index];
            m_buttons |= GamePad::getButtonsFromAxis(x, y, p_device->report->axis[i].type);
        }
    }

    // process hat
    if (p_device->report->hat.byte_index < len) {
        m_buttons |= GamePad::getButtonsFromHat(report[p_device->report->hat.byte_index]);
    }

    if (m_buttons != 0) TU_LOG1("%s: %s\r\n", p_device->name, Utility::toString(m_buttons).c_str());

    return true;
}

GamePad::PinMapping *PicoGamePad::getPinMapping() {
    return pinMapping;
}

void PicoGamePad::setLed(uint8_t type) {
    // TODO: handle type, ds4, etc...
}
