//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "pico_platform.h"
#include "pico_gamepad.h"
#include "pico_gfx.h"
#include "tusb.h"

using uGamePad::PicoPlatform;
using uGamePad::PicoGamePad;

PicoPlatform::PicoPlatform() {
    // setup pins
    // init output pins
    /*
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    pinMode(D9, OUTPUT);
    pinMode(D10, OUTPUT);
    pinMode(D11, OUTPUT);
    */

    // init usb host stack
    if (!tusb_init()) {
        printf("tusb_init failed...\r\n");
        while (true);
    }

    if (!tuh_init(0)) {
        printf("tuh_init failed...\r\n");
        while (true);
    }

    // init gamepad
    p_pad = new PicoGamePad();

    // init ui
    p_gfx = new PicoGfx();
}

void PicoPlatform::loop() {
    // handle usb host updates
    if (tuh_inited()) {
        tuh_task();
    } else {
        Debug.println("oops, tinyusb host service not inited...");
        while (true);
    }

    // handle led updates
    //Led::Update();
}
