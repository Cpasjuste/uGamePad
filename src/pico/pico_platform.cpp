//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "clock.h"

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
#if defined(DEBUG_RP2040_ZERO)
    // use pins 12 and 13 for tx/rx (b4/b5)
    Debug.setTX(D12); // vga pin 14 orange
    Debug.setRX(D13); // vga pin 15 (yellow)
#else
    // pico
    Debug.setTX(D16);
    Debug.setRX(D17);
#endif
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

static Clock m_clock;

static bool waitLatch() {
    // use a clock/timer, do not lock ourselves if something went wrong...
    m_clock.restart();
    while (!digitalRead(NES_LATCH)) {
        if (m_clock.getElapsedTime().asMilliseconds() > 20) {
            printf("waitLatch failed (LOW > 20 ms)\r\n");
            return false;
        }
    }

    m_clock.restart();
    while (digitalRead(NES_LATCH)) {
        if (m_clock.getElapsedTime().asMicroseconds() > 12) {
            printf("waitLatch failed (HIGH > 12 μs)\r\n");
            return false;
        }
    }

    return true;
}

static void waitClock() {
    // use a clock/timer, do not lock ourselves if something went wrong...
    //m_clock.restart();
    while (!digitalRead(NES_CLOCK)) {
        /*
        if (m_clock.getElapsedTime().asMilliseconds() > 1) {
            printf("waitClock failed (LOW > 1 ms)\r\n");
            return;
        }
        */
    }

    //m_clock.restart();
    while (digitalRead(NES_CLOCK)) {
        /*
        if (m_clock.getElapsedTime().asMilliseconds() > 1) {
            printf("waitClock failed (HIGH > 1 ms)\r\n");
            return;
        }
        */
    }
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

    // handle gamepad states
    if (!p_ui->isActive()) {
        // get gamepad sate
        buttons = p_pad->getButtons();

        // handle mvs mode
        if (p_pad->getMode() == GamePad::Mode::Mvs) {
            // only send buttons changed
            buttons_diff = buttons_old ^ buttons;
            buttons_old = buttons;
            if (buttons_diff) {
                GamePad::PinMapping *mapping = p_pad->getPinMapping();
                // generate pin output
                for (int i = 0; i < MAX_BUTTONS; i++) {
                    if (buttons_diff & mapping[i].button) {
                        digitalWrite(mapping[i].pin, buttons & mapping[i].button ? LOW : HIGH);
                    }
                }
            }
        } else { // handle nes mode
            // wait for latch, should be called every ~16ms for around 12μs
            //printf("latch wait...\r\n");
            if (!waitLatch()) {
                Platform::loop();
                return;
            }
            //printf("latched\r\n");

            digitalWrite(NES_DATA, buttons & GamePad::Button::B2 ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::B1 ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::SELECT ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::START ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::UP ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::DOWN ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::LEFT ? LOW : HIGH);
            waitClock();

            digitalWrite(NES_DATA, buttons & GamePad::Button::RIGHT ? LOW : HIGH);
            waitClock();
        }
    }

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
