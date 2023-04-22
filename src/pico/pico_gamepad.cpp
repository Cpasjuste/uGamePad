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

static PicoGamePad *s_picoGamePad = nullptr;
static volatile uint8_t m_clock_count = 0;

static GamePad::Output nes_output = {
#if defined(UGP_V10)
#if ARDUINO_RASPBERRY_PI_PICO
        // uGamePad v1.0 (rp2040-zero vga) pinout
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
#elif ARDUINO_SEEED_XIAO_RP2040
        .mode = GamePad::Mode::Nes,
        .mappings = {
                {GamePad::Button::B2,     NES_LATCH, -1,     -1}, // use interrupts
                {GamePad::Button::B1,     NES_CLOCK, -1,     -1}, // use interrupts
                {GamePad::Button::SELECT, NES_DATA,  OUTPUT, HIGH},
                {GamePad::Button::START,  UINT8_MAX, -1,     -1},
                {GamePad::Button::UP,     UINT8_MAX, -1,     -1},
                {GamePad::Button::DOWN,   UINT8_MAX, -1,     -1},
                {GamePad::Button::LEFT,   UINT8_MAX, -1,     -1},
                {GamePad::Button::RIGHT,  UINT8_MAX, -1,     -1},
        }
#endif
#endif
};

PicoGamePad::PicoGamePad() : GamePad() {
    s_picoGamePad = this;
    auto output = PicoGamePad::getOutputMode();

    if (output->mode == Mode::Nes) {
        attachInterrupt(digitalPinToInterrupt(NES_LATCH), onLatchRising, RISING);
        attachInterrupt(digitalPinToInterrupt(NES_CLOCK), onClockFalling, FALLING);
    } else {
        detachInterrupt(digitalPinToInterrupt(NES_LATCH));
        detachInterrupt(digitalPinToInterrupt(NES_CLOCK));
    }

    for (auto &mapping: output->mappings) {
        if (mapping.pin != UINT8_MAX) {
            pinMode(mapping.pin, mapping.pinMode);
            if (mapping.pinStatus != -1) {
                digitalWrite(mapping.pin, mapping.pinStatus);
            }
        }
    }
}

void PicoGamePad::onLatchRising() {
    Output *out = s_picoGamePad->getOutputMode();
    digitalWrite(NES_DATA, s_picoGamePad->getButtons() & out->mappings[m_clock_count].button ? LOW : HIGH);
    m_clock_count = 1;
}

void PicoGamePad::onClockFalling() {
    Output *out = s_picoGamePad->getOutputMode();
    digitalWrite(NES_DATA, s_picoGamePad->getButtons() & out->mappings[m_clock_count].button ? LOW : HIGH);
    m_clock_count++;
}

GamePad::Output *PicoGamePad::getOutputMode() {
    return &nes_output;
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

