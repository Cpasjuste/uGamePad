//
// Created by cpasjuste on 25/03/23.
//

#include <Arduino.h>

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
    for (int i = 0; i < MAX_BUTTONS; i++) {
        // TODO
    }

    return "";
}
