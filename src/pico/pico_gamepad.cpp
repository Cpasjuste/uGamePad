//
// Created by cpasjuste on 24/03/23.
//

//#include <Arduino.h>
#include <hardware/gpio.h>
#include "tusb.h"
#include "main.h"
#include "devices.h"
#include "pico_platform.h"
#include "pico_gamepad.h"

using namespace uGamePad;

static PicoGamePad *s_picoGamePad = nullptr;
static volatile uint8_t m_clock_count = 0;

PicoGamePad::PicoGamePad() : GamePad() {
    s_picoGamePad = this;

    // setup hardware buttons
    gpio_set_function(GPIO_BUTTON_UP, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_BUTTON_UP, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON_UP);
    gpio_set_function(GPIO_BUTTON_DOWN, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_BUTTON_DOWN, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON_DOWN);
    gpio_set_function(GPIO_BUTTON_ENTER, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_BUTTON_ENTER, GPIO_IN);
    gpio_pull_up(GPIO_BUTTON_ENTER);

    // setup output modes
    m_outputModes = {
            {
                    .name = "Jamma",
                    .mode = GamePad::Mode::Jamma,
                    .mappings = {
                            {GamePad::Button::B1, GPIO_BTN_B1, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::B2, GPIO_BTN_B2, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::B3, GPIO_BTN_B3, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::B4, GPIO_BTN_B4, GPIO_OUT, GPIO_HIGH},
#if !defined(UGP_DEBUG) || defined(UGP_DEV_BOARD) // used by tx/rx serial debug
                            {GamePad::Button::B5, GPIO_BTN_B5, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::B6, GPIO_BTN_B6, GPIO_OUT, GPIO_HIGH},
#endif
                            {GamePad::Button::SELECT, GPIO_BTN_SELECT, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::START, GPIO_BTN_START, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::UP, GPIO_BTN_UP, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::DOWN, GPIO_BTN_DOWN, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::LEFT, GPIO_BTN_LEFT, GPIO_OUT, GPIO_HIGH},
                            {GamePad::Button::RIGHT, GPIO_BTN_RIGHT, GPIO_OUT, GPIO_HIGH},
                    }
            },
#ifndef TODO_NES_SNES_MD_CABLES
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

#ifndef TODO_NES_SNES_MD_CABLES
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
#endif

void PicoGamePad::setOutputMode(const GamePad::Mode &mode) {
    GamePad::setOutputMode(mode);
    auto out = getOutputMode();
    if (out) {
        printf("PicoGamePad::setOutputMode: %s\r\n", out->name.c_str());
        // setup output pins
#ifndef TODO_NES_SNES_MD_CABLES
        if (out->mode == Mode::Nes || out->mode == Mode::Snes) {
            attachInterrupt(digitalPinToInterrupt(GPIO_NES_LATCH), onLatchRising, RISING);
            attachInterrupt(digitalPinToInterrupt(GPIO_NES_CLOCK), onClockFalling, FALLING);
        } else {
            detachInterrupt(digitalPinToInterrupt(GPIO_NES_LATCH));
            detachInterrupt(digitalPinToInterrupt(GPIO_NES_CLOCK));
        }
#endif
        for (auto &mapping: out->mappings) {
            if (mapping.pin != UINT8_MAX) {
                //pinMode(mapping.pin, mapping.pinMode);
                gpio_set_function(mapping.pin, GPIO_FUNC_SIO);
                gpio_set_dir(mapping.pin, mapping.direction);
                gpio_pull_up(GPIO_BUTTON_UP);
                if (mapping.defaultState != -1) {
                    gpio_put(mapping.pin, mapping.defaultState);
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

    //printf("uGamePad::loop: received data for '%s', len: %i)\r\n", p_device->name, len);
    /*
    for (int i = 0; i < len; i++) {
        printf("%08X", report[i]);
    }
    printf("\r\n");
    */

    // do not process bytes if less than x bytes
    if (len < p_device->data->min_report_size) return true;

    // reset buttons state
    uint16_t buttons = 0;

    // process buttons
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (p_device->data->buttons[i].byte >= len) continue;
        buttons |= report[p_device->data->buttons[i].byte] &
                   BIT(p_device->data->buttons[i].bit) ? (1 << i) : 0;
    }

    // process axis
    for (int i = 0; i < 3; i += 2) {
        if (p_device->data->axis[i].byte >= len) continue;
        if (p_device->data->axis[i].type & ReportData::AxisType::AXIS_I16) {
            int16_t x = (int16_t &) report[p_device->data->axis[i].byte];
            int16_t y = (int16_t &) report[p_device->data->axis[i + 1].byte];
            buttons |= GamePad::getButtonsFromAxis(x, y, p_device->data->axis[i].type);
        } else if (p_device->data->axis[i].type & ReportData::AxisType::AXIS_UI8) {
            uint8_t x = (uint8_t &) report[p_device->data->axis[i].byte];
            uint8_t y = (uint8_t &) report[p_device->data->axis[i + 1].byte];
            buttons |= GamePad::getButtonsFromAxis(x, y, p_device->data->axis[i].type);
        }
    }

    // process hat
    if (p_device->data->hat.byte < len) {
        buttons |= GamePad::getButtonsFromHat(report[p_device->data->hat.byte]);
    }

    // handle hardware buttons
    if (!gpio_get(GPIO_BUTTON_UP)) buttons |= GamePad::Button::UP;
    if (!gpio_get(GPIO_BUTTON_DOWN)) buttons |= GamePad::Button::DOWN;
    if (!gpio_get(GPIO_BUTTON_ENTER)) {
        buttons |= GamePad::Button::START;
        buttons |= GamePad::Button::MENU;
    }

    // for ui
    m_buttons = buttons;

    // handle gamepad states
    auto ui = getPlatform()->getUi();
    if (ui && !ui->isVisible()) {
        // get output mode/mapping
        GamePad::Output *output = getOutputMode();
        // handle jamma mode
        if (output->mode == GamePad::Mode::Jamma) {
            // set gpio states, only send buttons changed states
            m_buttons_diff = m_buttons_old ^ buttons;
            m_buttons_old = buttons;
#warning "TODO: remove debug code"
            if (1) {
                // generate pin output
                for (const auto &mapping: output->mappings) {
                    if (mapping.pin != UINT8_MAX /*&& m_buttons_diff & mapping.button*/) {
                        gpio_put(mapping.pin, buttons & mapping.button ? GPIO_LOW : GPIO_HIGH);
#ifndef NDEBUG
                        if (buttons & mapping.button)
                            printf("%s: %s (%i)\r\n", p_device->name,
                                   Utility::toString(mapping.button).c_str(), buttons & mapping.button ? 1 : 0);
#endif
                    }
                }
            }
        }
    }

    return true;
}
