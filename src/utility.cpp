//
// Created by cpasjuste on 25/03/23.
//

#ifndef NATIVE
#include <Arduino.h>
#if ARDUINO_ARCH_RP2040
#include <pico/bootrom.h>
#endif
#endif

#include <cmath>
#include <string>
#include "gamepad.h"
#include "utility.h"

using namespace uGamePad;

void Utility::reboot(bool bootloader) {
#ifndef NATIVE
    if (bootloader) {
#if ARDUINO_ARCH_RP2040
        reset_usb_boot(0, 0);
#elif ARDUINO_ARCH_SAMD
        *((volatile uint32_t *) (HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4)) = 0xf01669ef;
#endif
    }
    NVIC_SystemReset();
#endif
}

std::string Utility::toString(uint32_t buttons) {
    std::string ret;

    if (buttons & GamePad::Button::B1) ret += "1 ";
    if (buttons & GamePad::Button::B2) ret += "2 ";
    if (buttons & GamePad::Button::B3) ret += "3 ";
    if (buttons & GamePad::Button::B4) ret += "4 ";
    if (buttons & GamePad::Button::B5) ret += "5 ";
    if (buttons & GamePad::Button::B6) ret += "6 ";
    if (buttons & GamePad::Button::START) ret += "START ";
    if (buttons & GamePad::Button::SELECT) ret += "SELECT ";
    if (buttons & GamePad::Button::UP) ret += "UP ";
    if (buttons & GamePad::Button::DOWN) ret += "DOWN ";
    if (buttons & GamePad::Button::LEFT) ret += "LEFT ";
    if (buttons & GamePad::Button::RIGHT) ret += "RIGHT ";

    return ret;
}

Utility::Matrix2D Utility::rotationMatrix(float angle) {
    Matrix2D matrix{};
    matrix.m11 = std::cos(angle);
    matrix.m12 = -std::sin(angle);
    matrix.m21 = std::sin(angle);
    matrix.m22 = std::cos(angle);
    return matrix;
}

Utility::Vector2f Utility::transformPoint(Utility::Vector2f point, Utility::Matrix2D matrix) {
    Vector2f result{};
    result.x = point.x * matrix.m11 + point.y * matrix.m12;
    result.y = point.x * matrix.m21 + point.y * matrix.m22;
    return result;
}
