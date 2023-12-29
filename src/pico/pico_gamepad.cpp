//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "devices.h"
#include "utility/utility.h"
#include "pico_gamepad.h"

using namespace uGamePad;

static PicoGamePad *s_picoGamePad = nullptr;
static volatile uint8_t m_clock_count = 0;

PicoGamePad::PicoGamePad() : GamePad() {
    s_picoGamePad = this;

    // setup hardware buttons
    pinMode(GPIO_BUTTON_UP, INPUT_PULLUP);
    pinMode(GPIO_BUTTON_DOWN, INPUT_PULLUP);
    pinMode(GPIO_BUTTON_ENTER, INPUT_PULLUP);

    // setup output modes
    m_outputModes = {
            {
                    .name = "Jamma",
                    .mode = GamePad::Mode::Jamma,
                    .mappings = {
                            {GamePad::Button::B1, GPIO_BTN_B1, OUTPUT, HIGH},
                            {GamePad::Button::B2, GPIO_BTN_B2, OUTPUT, HIGH},
                            {GamePad::Button::B3, GPIO_BTN_B3, OUTPUT, HIGH},
                            {GamePad::Button::B4, GPIO_BTN_B4, OUTPUT, HIGH},
#if !defined(UGP_DEBUG) || defined(UGP_DEV_BOARD) // used by tx/rx serial debug
                            {GamePad::Button::B5, GPIO_BTN_B5, OUTPUT, HIGH},
                            {GamePad::Button::B6, GPIO_BTN_B6, OUTPUT, HIGH},
#endif
                            {GamePad::Button::SELECT, GPIO_BTN_SELECT, OUTPUT, HIGH},
                            {GamePad::Button::START, GPIO_BTN_START, OUTPUT, HIGH},
                            {GamePad::Button::UP, GPIO_BTN_UP, OUTPUT, HIGH},
                            {GamePad::Button::DOWN, GPIO_BTN_DOWN, OUTPUT, HIGH},
                            {GamePad::Button::LEFT, GPIO_BTN_LEFT, OUTPUT, HIGH},
                            {GamePad::Button::RIGHT, GPIO_BTN_RIGHT, OUTPUT, HIGH},
                    }
            },
#ifndef TODO_NES_SNES_CABLES
            {
                    .name = "Nes",
                    .mode = GamePad::Mode::Nes,
                    .mappings = {
                            {GamePad::Button::B2,     GPIO_NES_LATCH, -1,     -1},   // use interrupts
                            {GamePad::Button::B1,     GPIO_NES_CLOCK, -1,     -1},   // use interrupts
                            {GamePad::Button::SELECT, GPIO_NES_DATA,  OUTPUT, HIGH},
                            {GamePad::Button::START,  UINT8_MAX,      -1,     -1},
                            {GamePad::Button::UP,     UINT8_MAX,      -1,     -1},
                            {GamePad::Button::DOWN,   UINT8_MAX,      -1,     -1},
                            {GamePad::Button::LEFT,   UINT8_MAX,      -1,     -1},
                            {GamePad::Button::RIGHT,  UINT8_MAX,      -1,     -1},
                    }
            },
            {
                    .name = "Snes",
                    .mode = GamePad::Mode::Snes,
                    .mappings = {
                            {GamePad::Button::B2,     GPIO_NES_LATCH, -1,     -1},   // use interrupts
                            {GamePad::Button::B1,     GPIO_NES_CLOCK, -1,     -1},   // use interrupts
                            {GamePad::Button::SELECT, GPIO_NES_DATA,  OUTPUT, HIGH},
                            {GamePad::Button::START,  UINT8_MAX,      -1,     -1},
                            {GamePad::Button::UP,     UINT8_MAX,      -1,     -1},
                            {GamePad::Button::DOWN,   UINT8_MAX,      -1,     -1},
                            {GamePad::Button::LEFT,   UINT8_MAX,      -1,     -1},
                            {GamePad::Button::RIGHT,  UINT8_MAX,      -1,     -1},
                    }
            },
            {
                    .name = "MD",
                    .mode = GamePad::Mode::Md,
                    .mappings = {
                            // TODO
                    }
            }
#endif
    };

    // setup output pins
    PicoGamePad::setOutputMode(m_outputMode);

    // update hardware buttons states
    PicoGamePad::loop();
}

void PicoGamePad::onLatchRising() {
    Output *out = s_picoGamePad->getOutputMode();
    digitalWrite(GPIO_NES_DATA, s_picoGamePad->getButtons() & out->mappings[m_clock_count].button ? LOW : HIGH);
    m_clock_count = 1;
}

void PicoGamePad::onClockFalling() {
    Output *out = s_picoGamePad->getOutputMode();
    digitalWrite(GPIO_NES_DATA, s_picoGamePad->getButtons() & out->mappings[m_clock_count].button ? LOW : HIGH);
    m_clock_count++;
}

void PicoGamePad::setOutputMode(const GamePad::Mode &mode) {
    GamePad::setOutputMode(mode);
    auto out = getOutputMode();
    if (out) {
        printf("PicoGamePad::setOutputMode: %s\r\n", out->name.c_str());
        // setup output pins
        if (out->mode == Mode::Nes || out->mode == Mode::Snes) {
            attachInterrupt(digitalPinToInterrupt(GPIO_NES_LATCH), onLatchRising, RISING);
            attachInterrupt(digitalPinToInterrupt(GPIO_NES_CLOCK), onClockFalling, FALLING);
        } else {
            detachInterrupt(digitalPinToInterrupt(GPIO_NES_LATCH));
            detachInterrupt(digitalPinToInterrupt(GPIO_NES_CLOCK));
        }

        for (auto &mapping: out->mappings) {
            if (mapping.pin != UINT8_MAX) {
                pinMode(mapping.pin, mapping.pinMode);
                if (mapping.pinStatus != -1) {
                    digitalWrite(mapping.pin, mapping.pinStatus);
                }
            }
        }
    }
}

bool PicoGamePad::report(const uint8_t *report, uint16_t len) {
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
                     BIT(p_device->data->buttons[i].bit) ? (1 << i) : 0;
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

    if (m_buttons != 0) printf("%s: %s\r\n", p_device->name, Utility::toString(m_buttons).c_str());

    return true;
}

void PicoGamePad::loop() {
    // handle hardware buttons
    if (!digitalRead(GPIO_BUTTON_UP)) m_buttons |= GamePad::Button::UP;
    if (!digitalRead(GPIO_BUTTON_DOWN)) m_buttons |= GamePad::Button::DOWN;
    if (!digitalRead(GPIO_BUTTON_ENTER)) {
        m_buttons |= GamePad::Button::START;
        m_buttons |= GamePad::Button::MENU;
    }

    // handle gamepad states
    auto ui = getPlatform()->getUi();
    if (ui && !ui->isVisible()) {
        // get output mode/mapping
        GamePad::Output *output = getOutputMode();
        // handle jamma mode
        if (output->mode == GamePad::Mode::Jamma) {
            // get gamepad sate
            uint16_t buttons = getButtons();
            // only send buttons changed states
            m_buttons_diff = m_buttons_old ^ buttons;
            m_buttons_old = buttons;
            if (m_buttons_diff) {
                // generate pin output
                for (const auto &mapping: output->mappings) {
                    if (mapping.pin != UINT8_MAX && m_buttons_diff & mapping.button) {
                        digitalWrite(mapping.pin, buttons & mapping.button ? LOW : HIGH);
                    }
                }
            }
        }
    }

    GamePad::loop();
}
