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
    gpio_set_function(GPIO_HW_BTN_UP, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_HW_BTN_UP, GPIO_IN);
    gpio_pull_up(GPIO_HW_BTN_UP);
    gpio_set_function(GPIO_HW_BTN_DOWN, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_HW_BTN_DOWN, GPIO_IN);
    gpio_pull_up(GPIO_HW_BTN_DOWN);
    gpio_set_function(GPIO_HW_BTN_ENTER, GPIO_FUNC_SIO);
    gpio_set_dir(GPIO_HW_BTN_ENTER, GPIO_IN);
    gpio_pull_up(GPIO_HW_BTN_ENTER);

    // setup output modes
    m_outputModes = {
        {
            .name = "Jamma",
            .mode = Jamma,
            .mappings = {
                {B1, GPIO_BTN_B1, GPIO_OUT, GPIO_HIGH},
                {B2, GPIO_BTN_B2, GPIO_OUT, GPIO_HIGH},
                {B3, GPIO_BTN_B3, GPIO_OUT, GPIO_HIGH},
                {B4, GPIO_BTN_B4, GPIO_OUT, GPIO_HIGH},
#if !defined(UGP_DEBUG) || defined(UGP_DEV_BOARD) // used by tx/rx serial debug
                {B5, GPIO_BTN_B5, GPIO_OUT, GPIO_HIGH},
                {B6, GPIO_BTN_B6, GPIO_OUT, GPIO_HIGH},
#endif
                {SELECT, GPIO_BTN_SELECT, GPIO_OUT, GPIO_HIGH},
                {START, GPIO_BTN_START, GPIO_OUT, GPIO_HIGH},
                {UP, GPIO_BTN_UP, GPIO_OUT, GPIO_HIGH},
                {DOWN, GPIO_BTN_DOWN, GPIO_OUT, GPIO_HIGH},
                {LEFT, GPIO_BTN_LEFT, GPIO_OUT, GPIO_HIGH},
                {RIGHT, GPIO_BTN_RIGHT, GPIO_OUT, GPIO_HIGH},
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
    const auto out = getOutputMode();
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
                gpio_pull_up(mapping.pin);
                if (mapping.defaultState != -1) {
                    gpio_put(mapping.pin, mapping.defaultState);
                }
            }
        }
    }
}

uint32_t PicoGamePad::getHardwareButtons() {
    // handle hardware buttons
    uint32_t buttons = 0;
    if (!gpio_get(GPIO_HW_BTN_UP)) buttons |= UP;
    if (!gpio_get(GPIO_HW_BTN_DOWN)) buttons |= DOWN;
    if (!gpio_get(GPIO_HW_BTN_ENTER)) {
        buttons |= START;
        buttons |= MENU;
    }

    if (buttons) printf("getHardwareButtons: %u\r\n", buttons);

    return buttons;
}

bool PicoGamePad::onHidReport(const uint8_t *report, uint16_t len) {
    // process report in base class
    GamePad::onHidReport(report, len);

    // update hardware buttons
    m_buttons |= getHardwareButtons();

    // handle gamepad states
    const auto ui = getPlatform()->getUi();
    if (ui && !ui->isVisible()) {
        // get output mode/mapping
        const auto output = getOutputMode();
        // handle jamma mode
        if (output->mode == GamePad::Mode::Jamma) {
            // set gpio states, only send buttons changed states
            m_buttons_diff = m_buttons_old ^ m_buttons;
            m_buttons_old = m_buttons;
            if (m_buttons_diff) {
                // generate pin output
                for (const auto &mapping: output->mappings) {
                    if (mapping.pin != UINT8_MAX && m_buttons_diff & mapping.button) {
                        gpio_put(mapping.pin, m_buttons & mapping.button ? GPIO_LOW : GPIO_HIGH);
#ifndef NDEBUG
                        printf("%s: %s (%i)\r\n", p_device->name,
                               Utility::toString(mapping.button).c_str(), m_buttons & mapping.button ? 1 : 0);
#endif
                    }
                }
            }
        }
    }

    return true;
}
