//
// Created by cpasjuste on 25/03/23.
//

#ifndef NATIVE
#include <Arduino.h>
#if ARDUINO_ARCH_RP2040
#include <pico/bootrom.h>
#endif
#endif

#include <string>
#include "gamepad.h"
#include "utility.h"

using namespace uGamePad;

void Utility::reboot(bool bootloader) {
#ifndef NATIVE
    if (bootloader) {
#if ARDUINO_ARCH_RP2040
        reset_usb_boot(0, 0);
#else
        printf("Utility::reboot: reboot to bootloader not implemented...");
#endif
    }
    NVIC_SystemReset();
#endif
}

std::string Utility::toString(uint32_t buttons) {
    std::string ret;

    if (buttons & GamePad::Button::B1) ret += "B1 ";
    if (buttons & GamePad::Button::B2) ret += "B2 ";
    if (buttons & GamePad::Button::B3) ret += "B3 ";
    if (buttons & GamePad::Button::B4) ret += "B4 ";
    if (buttons & GamePad::Button::B5) ret += "B5 ";
    if (buttons & GamePad::Button::B6) ret += "B6 ";
    if (buttons & GamePad::Button::START) ret += "START ";
    if (buttons & GamePad::Button::SELECT) ret += "SELECT ";
    if (buttons & GamePad::Button::UP) ret += "UP ";
    if (buttons & GamePad::Button::DOWN) ret += "DOWN ";
    if (buttons & GamePad::Button::LEFT) ret += "LEFT ";
    if (buttons & GamePad::Button::RIGHT) ret += "RIGHT ";

    return ret;
}
