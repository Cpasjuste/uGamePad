//
// Created by cpasjuste on 25/03/23.
//

#ifndef LINUX

#include <pico/bootrom.h>
#include <hardware/watchdog.h>

#endif

#include <string>
#include "gamepad.h"
#include "utility.h"

using namespace uGamePad;

void Utility::reboot(bool bootloader) {
#ifndef LINUX
    if (bootloader) {
#if PICO_BUILD
        reset_usb_boot(0, 0);
#else
        printf("Utility::reboot: reboot to bootloader not implemented...");
#endif
    }
    watchdog_reboot(0, 0, 50);
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

std::string Utility::parseSize(uint64_t size) {
    char output[32];
    const char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);
    auto dblBytes = (double) size;
    int i = 0;

    if (size <= 0) {
        return "0 B";
    } else {
        if (size > 1024) {
            for (i = 0; (size / 1024) > 0 && i < length - 1; i++, size /= 1024)
                dblBytes = (double) size / 1024.0;
        }

        snprintf(output, 31, "~%i %s", (int) dblBytes, suffix[i]);
        return output;
    }
}

std::string Utility::baseName(const std::string &path) {
    std::string name = path;
    if (path.size() > 1) {
        const size_t idx = path.find_last_of('/');
        if (idx != std::string::npos) {
            name.erase(0, idx + 1);
        }
    }
    return name;
}
