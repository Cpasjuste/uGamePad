//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"

using namespace uGamePad;

#ifdef __arm__
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

PicoPlatform::PicoPlatform() = default;

void uGamePad::PicoPlatform::setup() {
    // setup serial debug
#if defined(ARDUINO_RASPBERRY_PI_PICO)
    Debug.setTX(D16);
    Debug.setRX(D17);
#endif
    Debug.begin(115200);

    // init filesystem
    p_fs = new PicoFs();

    // init gfx
    p_gfx = new PicoGfx();

    // init gamepad
    p_pad = new PicoGamePad();

    // init usb host stack
    if (!tusb_init()) {
        printf("tusb_init failed...\r\n");
        while (true);
    }
    if (!tuh_init(0)) {
        printf("tuh_init failed...\r\n");
        while (true);
    }

    // all done
    Platform::setup();
}

void PicoPlatform::loop() {
    // handle usb host updates
    if (tuh_inited()) {
        tuh_task();
    } else {
        printf("oops, tinyusb host service not inited...\r\n");
        while (true);
    }

    // handle led updates
    //Led::Update();

    Platform::loop();
}

int PicoPlatform::getFreeHeap() {
    char top;
#ifdef __arm__
    return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
    return &top - __brkval;
#else  // __arm__
    return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
