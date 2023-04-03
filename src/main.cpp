//
// Created by cpasjuste on 29/03/23.
//

#ifdef NATIVE
#include "linux_platform.h"
#else
#include "pico_platform.h"
#endif

using namespace uGamePad;

#if NATIVE
LinuxPlatform platform;
#else
PicoPlatform platform;
#endif
uint16_t buttons;
uint16_t buttons_old;

Platform *getPlatform() {
#if NATIVE
    return (LinuxPlatform *) &platform;
#else
    return (PicoPlatform *) &platform;
#endif
}

void setup() {
    // setup serial debug
#if defined(ARDUINO_RASPBERRY_PI_PICO)
    Debug.setTX(D16);
    Debug.setRX(D17);
#endif
    Debug.begin(115200);

    // motd
    printf("        _____                      _____          _ \r\n"
           "       / ____|                    |  __ \\        | |\r\n"
           " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
           "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
           "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
           " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n\n");
    fflush(stdout);
}

void loop() {
    platform.loop();

    // get gamepad sate
    buttons = platform.getPad()->getButtons();
    // only loop on button change
    bool changed = buttons_old ^ buttons;
    buttons_old = buttons;
    if (changed) {
#ifndef NATIVE
        GamePad::PinMapping *mapping = platform.getPad()->getPinMapping();
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
