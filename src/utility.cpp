//
// Created by cpasjuste on 25/03/23.
//

#include <Arduino.h>
#include <string>

#if ARDUINO_ARCH_RP2040

#include <pico/bootrom.h>

#endif

#include "gamepad.h"
#include "utility.h"

void Utility::reboot(bool bootloader) {
    if (bootloader) {
#if ARDUINO_ARCH_RP2040
        reset_usb_boot(0, 0);
#elif ARDUINO_ARCH_SAMD
        *((volatile uint32_t *) (HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4)) = 0xf01669ef;
#endif
    }
    NVIC_SystemReset();
}

std::string Utility::toString(uint32_t buttons) {
    std::string ret;

    if (buttons & uGamePad::Button::B_1) ret += "B_1 ";
    if (buttons & uGamePad::Button::B_2) ret += "B_2 ";
    if (buttons & uGamePad::Button::B_3) ret += "B_3 ";
    if (buttons & uGamePad::Button::B_4) ret += "B_4 ";
    if (buttons & uGamePad::Button::B_5) ret += "B_5 ";
    if (buttons & uGamePad::Button::B_6) ret += "B_6 ";
    if (buttons & uGamePad::Button::B_START) ret += "B_START ";
    if (buttons & uGamePad::Button::B_SELECT) ret += "B_SELECT ";
    if (buttons & uGamePad::Button::B_UP) ret += "B_UP ";
    if (buttons & uGamePad::Button::B_DOWN) ret += "B_DOWN ";
    if (buttons & uGamePad::Button::B_LEFT) ret += "B_LEFT ";
    if (buttons & uGamePad::Button::B_RIGHT) ret += "B_RIGHT ";

    return ret;
}
