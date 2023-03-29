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

    if (buttons & uGamePad::Button::B1) ret += "1 ";
    if (buttons & uGamePad::Button::B2) ret += "2 ";
    if (buttons & uGamePad::Button::B3) ret += "3 ";
    if (buttons & uGamePad::Button::B4) ret += "4 ";
    if (buttons & uGamePad::Button::B5) ret += "5 ";
    if (buttons & uGamePad::Button::B6) ret += "6 ";
    if (buttons & uGamePad::Button::START) ret += "START ";
    if (buttons & uGamePad::Button::SELECT) ret += "SELECT ";
    if (buttons & uGamePad::Button::UP) ret += "UP ";
    if (buttons & uGamePad::Button::DOWN) ret += "DOWN ";
    if (buttons & uGamePad::Button::LEFT) ret += "LEFT ";
    if (buttons & uGamePad::Button::RIGHT) ret += "RIGHT ";

    return ret;
}
