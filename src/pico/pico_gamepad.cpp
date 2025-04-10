//
// Created by cpasjuste on 24/03/23.
//

#include <hardware/gpio.h>
#include "tusb.h"
#include "main.h"
#include "devices.h"
#include "pico_platform.h"
#include "pico_gamepad.h"

using namespace uGamePad;

static PicoGamePad *s_picoGamePad = nullptr;
static volatile uint8_t m_clock_count = 0;

PicoGamePad::PicoGamePad() {
    s_picoGamePad = this;

    // setup hardware buttons
    if (GPIO_HW_BTN_UP != INPUT_DUMMY) {
        gpio_set_function(GPIO_HW_BTN_UP, GPIO_FUNC_SIO);
        gpio_set_dir(GPIO_HW_BTN_UP, GPIO_IN);
        gpio_pull_up(GPIO_HW_BTN_UP);
    }

    if (GPIO_HW_BTN_DOWN != INPUT_DUMMY) {
        gpio_set_function(GPIO_HW_BTN_DOWN, GPIO_FUNC_SIO);
        gpio_set_dir(GPIO_HW_BTN_DOWN, GPIO_IN);
        gpio_pull_up(GPIO_HW_BTN_DOWN);
    }

    if (GPIO_HW_BTN_ENTER != INPUT_DUMMY) {
        gpio_set_function(GPIO_HW_BTN_ENTER, GPIO_FUNC_SIO);
        gpio_set_dir(GPIO_HW_BTN_ENTER, GPIO_IN);
        gpio_pull_up(GPIO_HW_BTN_ENTER);
    }

    // setup output modes
    m_outputModes = {
        {
            .name = "Jamma",
            .mode = Jamma,
            .mappings = {
                {B1, GPIO_BTN_B1, GPIO_IN, GPIO_LOW}, // BSS138PS
                {B2, GPIO_BTN_B2, GPIO_IN, GPIO_HIGH}, // ULN2803A
                {B3, GPIO_BTN_B3, GPIO_IN, GPIO_LOW}, // BSS138PS
                {B4, GPIO_BTN_B4, GPIO_IN, GPIO_HIGH}, // ULN2803A
#if !defined(UGP_DEBUG) || defined(UGP_DEV_BOARD) // used by tx/rx serial debug
                {B5, GPIO_BTN_B5, GPIO_IN, GPIO_LOW}, // BSS138PS
                {B6, GPIO_BTN_B6, GPIO_IN, GPIO_HIGH}, // ULN2803A
#endif
                {SELECT, GPIO_BTN_SELECT, GPIO_IN, GPIO_HIGH}, // ULN2803A
                {START, GPIO_BTN_START, GPIO_IN, GPIO_LOW}, // BSS138PS
                {UP, GPIO_BTN_UP, GPIO_IN, GPIO_HIGH}, // ULN2803A
                {DOWN, GPIO_BTN_DOWN, GPIO_IN, GPIO_HIGH}, // ULN2803A
                {LEFT, GPIO_BTN_LEFT, GPIO_IN, GPIO_HIGH}, // ULN2803A
                {RIGHT, GPIO_BTN_RIGHT, GPIO_IN, GPIO_HIGH}, // ULN2803A
            }
        },
        {
            .name = "Nes",
            .mode = Nes,
            .mappings = {
                {B2, GPIO_NES_LATCH, GPIO_IN}, // use interrupts
                {B1, GPIO_NES_CLOCK, GPIO_IN}, // use interrupts
                {SELECT, GPIO_NES_DATA, GPIO_OUT}, // use interrupts
                {START, INPUT_DUMMY, GPIO_IN},
                {UP, INPUT_DUMMY, GPIO_IN},
                {DOWN, INPUT_DUMMY, GPIO_IN},
                {LEFT, INPUT_DUMMY, GPIO_IN},
                {RIGHT, INPUT_DUMMY, GPIO_IN},
            }
        },
#if 0
        {
            .name = "Snes",
            .mode = Snes,
            .mappings = {
                {B2, GPIO_NES_LATCH, GPIO_IN, GPIO_FLOAT}, // use interrupts
                {B1, GPIO_NES_CLOCK, GPIO_IN, GPIO_FLOAT}, // use interrupts
                {SELECT, GPIO_NES_DATA, GPIO_OUT, GPIO_HIGH},
                {START, INPUT_DUMMY, GPIO_IN, GPIO_FLOAT},
                {UP, INPUT_DUMMY, GPIO_IN, GPIO_FLOAT},
                {DOWN, INPUT_DUMMY, GPIO_IN, GPIO_FLOAT},
                {LEFT, INPUT_DUMMY, GPIO_IN, GPIO_FLOAT},
                {RIGHT, INPUT_DUMMY, GPIO_IN, GPIO_FLOAT},
            }
        },
        {
            .name = "MD",
            .mode = Md,
            .mappings = {
                // TODO
            }
        }
#endif
    };

    // setup output pins
    PicoGamePad::setOutputMode(m_outputMode);
}

static void onNesCallback(const uint gpio, const uint32_t events) {
    if (gpio == GPIO_NES_LATCH) {
        m_clock_count = 0; // reset
    }

    //printf("onNesCallback: %i 0x%04X\r\n", gpio, events);

    // send button value to data pin
    gpio_put(GPIO_NES_DATA, s_picoGamePad->getButtons()
                            & s_picoGamePad->getOutputMode()->mappings[m_clock_count].button
                                ? GPIO_LOW
                                : GPIO_HIGH);

    // increment on GPIO_NES_LATCH / GPIO_NES_CLOCK
    m_clock_count++;
}

void PicoGamePad::setOutputMode(const Mode &mode) {
    GamePad::setOutputMode(mode);
    const auto out = getOutputMode();
    if (out) {
        printf("PicoGamePad::setOutputMode: %s\r\n", out->name.c_str());
        // setup output pins
        if (out->mode == Nes) {
            gpio_set_irq_enabled_with_callback(GPIO_NES_LATCH, GPIO_IRQ_EDGE_RISE, true, &onNesCallback);
            gpio_set_irq_enabled_with_callback(GPIO_NES_CLOCK, GPIO_IRQ_EDGE_FALL, true, &onNesCallback);
        } else {
            gpio_set_irq_enabled_with_callback(GPIO_NES_LATCH, GPIO_IRQ_EDGE_RISE, false, nullptr);
            gpio_set_irq_enabled_with_callback(GPIO_NES_CLOCK, GPIO_IRQ_EDGE_FALL, false, nullptr);
        }

        for (const auto &mapping: out->mappings) {
            if (mapping.pin != INPUT_DUMMY) {
                gpio_set_function(mapping.pin, GPIO_FUNC_SIO);
                gpio_set_dir(mapping.pin, mapping.direction);
            }
        }
    }
}

uint32_t PicoGamePad::getHardwareButtons() {
    uint32_t buttons = 0;

    // handle hardware buttons
    if (GPIO_HW_BTN_UP != INPUT_DUMMY && !gpio_get(GPIO_HW_BTN_UP)) buttons |= DPAD_UP;
    if (GPIO_HW_BTN_DOWN != INPUT_DUMMY && !gpio_get(GPIO_HW_BTN_DOWN)) buttons |= DPAD_DOWN;
    if (GPIO_HW_BTN_ENTER != INPUT_DUMMY && !gpio_get(GPIO_HW_BTN_ENTER)) buttons |= MENU;

    return buttons;
}

bool PicoGamePad::onHidReport(const uint8_t *report, uint16_t len) {
    // process report in base class
    GamePad::onHidReport(report, len);

    // handle gpio states
    const auto ui = getPlatform()->getUi();
    if (!ui) return false; // app not ready...

    if (!ui->isVisible()) {
        // get output mode/mapping
        const auto output = getOutputMode();
        // handle jamma mode
        if (output->mode == Jamma) {
            // set gpio states, only send buttons changed states
            m_buttons_diff = m_buttons_old ^ m_buttons;
            m_buttons_old = m_buttons;
            if (m_buttons_diff) {
                // generate pin output
                for (const auto &mapping: output->mappings) {
                    if (mapping.pin != INPUT_DUMMY && m_buttons_diff & mapping.button) {
                        // open-drain simulation
                        if (m_buttons & mapping.button) {
                            gpio_set_dir(mapping.pin, GPIO_OUT);
                            gpio_put(mapping.pin, mapping.logic);
                        } else {
                            gpio_set_dir(mapping.pin, GPIO_IN);
                        }
#ifndef NDEBUG
                        printf("%s: %s (%i)\r\n", p_device->name,
                               Utility::parseButtons(mapping.button).c_str(), m_buttons & mapping.button ? 1 : 0);
#endif
                    }
                }
            }
        }
    } else {
        // clear buttons/gpio states if ui is visible
        const auto output = getOutputMode();
        if (output->mode == Jamma) {
            for (const auto &mapping: output->mappings) {
                if (mapping.pin != INPUT_DUMMY && gpio_get_dir(mapping.pin) != GPIO_IN) {
                    gpio_set_dir(mapping.pin, GPIO_IN);
                }
            }
        }
    }

    return true;
}
