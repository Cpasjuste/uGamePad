//
// Created by cpasjuste on 29/03/23.
//

#include "main.h"

using namespace uGamePad;

Platform *platform;
uint16_t buttons;
uint16_t buttons_old;

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
    bool changed = buttons_old ^ buttons;
    buttons_old = buttons;
    if (changed) {
#ifndef NATIVE
        GamePad::PinMapping *mapping = platform->getPad()->getPinMapping();
        if (mapping) {
            // generate pin output
            for (int i = 0; i < MAX_BUTTONS; i++) {
                //digitalWrite(mapping[i].pin, pad.buttons & mapping[i].button ? HIGH : LOW);
            }
        }
#endif
    }

    delay(1);
}
