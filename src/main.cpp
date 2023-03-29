//
// Created by cpasjuste on 29/03/23.
//

#ifdef NATIVE
#include "linux_platform.h"
#else
#include "pico_platform.h"
#endif

using uGamePad::Platform;

Platform *platform;
uint16_t buttons;
uint16_t buttons_old;

void setup() {
#if NATIVE
    platform = (Platform *) new uGamePad::LinuxPlatform();
#else
    platform = (Platform *) new uGamePad::PicoPlatform();
#endif

    // motd
    printf("        _____                      _____          _ \r\n"
           "       / ____|                    |  __ \\        | |\r\n"
           " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
           "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
           "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
           " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n\n");

    // ui splash
    platform->getUi()->drawSplash();
}

void loop() {
    platform->loop();

    // get gamepad sate
    buttons = platform->getPad()->getButtons();
    // only update on button change
    bool changed = buttons_old ^ buttons;
    buttons_old = buttons;
    if (changed) {
#ifndef NATIVE
        uGamePad::GamePad::PinMapping *mapping = platform->getPad()->getPinMapping();
        if (mapping) {
            // generate pin output
            for (int i = 0; i < MAX_BUTTONS; i++) {
                //digitalWrite(mapping[i].pin, pad.buttons & mapping[i].button ? HIGH : LOW);
            }
        }
#endif
    }

#if !NATIVE
    delay(1);
#endif
}

#if NATIVE
int main() {
    setup();
    while (true) {
        loop();
    }
}
#endif
