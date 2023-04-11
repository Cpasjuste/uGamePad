//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"

using namespace uGamePad;

Platform *platform;
uint16_t buttons, buttons_old, buttons_diff;

Platform *getPlatform() {
    return platform;
}

void setup() {
#if NATIVE
    platform = new LinuxPlatform();
#else
    platform = new PicoPlatform();
#endif
    platform->setup();
}

void loop() {
    platform->loop();

    // get gamepad sate
    buttons = platform->getPad()->getButtons();

    // only loop on button change
    buttons_diff = buttons_old ^ buttons;
    buttons_old = buttons;
    if (buttons_diff) {
#ifndef NATIVE
        GamePad::PinMapping *mapping = platform->getPad()->getPinMapping();
        // generate pin output
        for (int i = 0; i < MAX_BUTTONS; i++) {
            if (buttons_diff & mapping[i].button) {
                digitalWrite(mapping[i].pin, buttons & mapping[i].button ? HIGH : LOW);
            }
        }
#endif
    }

    delay(1);
}
