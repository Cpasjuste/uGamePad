//
// Created by cpasjuste on 24/03/23.
//

#include "linux_gamepad.h"

using uGamePad::GamePad;
using uGamePad::LinuxGamePad;

bool LinuxGamePad::update(const uint8_t *report, uint16_t len) {
    return true;
}
