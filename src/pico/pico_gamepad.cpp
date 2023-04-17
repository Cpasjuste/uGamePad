//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "devices.h"
#include "utility.h"
#include "pico_gamepad.h"


using namespace uGamePad;

// RP2040-Zero pinout
static GamePad::PinMapping pinMapping[MAX_BUTTONS] = {
#if UGP_V10
        {GamePad::Button::B1, D9},
        {GamePad::Button::B2, D10},
        {GamePad::Button::B3, D11},
        {GamePad::Button::B4, D12},
        {GamePad::Button::B5, D3},
        {GamePad::Button::B6, D8},
        {GamePad::Button::SELECT, D7},
        {GamePad::Button::START, D15},
        {GamePad::Button::UP, D26},
        {GamePad::Button::DOWN, D27},
        {GamePad::Button::LEFT, D29},
        {GamePad::Button::RIGHT, D28},
#endif
};

PicoGamePad::PicoGamePad() : GamePad() {
    // enable pin output
    for (auto &i: pinMapping) {
        pinMode(i.pin, OUTPUT);
        // mvs use pull-up pins it seems
        digitalWrite(i.pin, HIGH);
    }
}

bool PicoGamePad::update(const uint8_t *report, uint16_t len) {
    if (!p_device || !p_device->data) {
        TU_LOG1("uGamePad::loop: error: device not set\r\n");
        return false;
    }

    TU_LOG2("uGamePad::loop: received data for '%s': type: %i, len: %i)\r\n",
            p_device->name, p_device->type, len);

    // do not process bytes if less than x bytes
    if (len < p_device->data->min_report_size) return true;

    // reset buttons state
    m_buttons = 0;

    // process buttons
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (p_device->data->buttons[i].byte >= len) continue;
        m_buttons |= report[p_device->data->buttons[i].byte] &
                     p_device->data->buttons[i].bit ? (1 << i) : 0;
    }

    // process axis
    for (int i = 0; i < 3; i += 2) {
        if (p_device->data->axis[i].byte >= len) continue;
        if (p_device->data->axis[i].type & ReportData::AxisType::AXIS_I16) {
            int16_t x = (int16_t &) report[p_device->data->axis[i].byte];
            int16_t y = (int16_t &) report[p_device->data->axis[i + 1].byte];
            m_buttons |= GamePad::getButtonsFromAxis(x, y, p_device->data->axis[i].type);
        } else if (p_device->data->axis[i].type & ReportData::AxisType::AXIS_UI8) {
            uint8_t x = (uint8_t &) report[p_device->data->axis[i].byte];
            uint8_t y = (uint8_t &) report[p_device->data->axis[i + 1].byte];
            m_buttons |= GamePad::getButtonsFromAxis(x, y, p_device->data->axis[i].type);
        }
    }

    // process hat
    if (p_device->data->hat.byte < len) {
        m_buttons |= GamePad::getButtonsFromHat(report[p_device->data->hat.byte]);
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
