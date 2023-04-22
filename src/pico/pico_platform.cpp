//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "clock.h"

using namespace uGamePad;

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
        // get output mode/mapping
        GamePad::Output *output = p_pad->getOutputMode();

        // handle mvs mode
        if (output->mode == GamePad::Mode::Mvs) {
            // get gamepad sate
            uint16_t buttons = p_pad->getButtons();

            // only send buttons changed
            m_buttons_diff = m_buttons_old ^ buttons;
            m_buttons_old = buttons;
            if (m_buttons_diff) {
                // generate pin output
                for (const auto &mapping: output->mappings) {
                    if (m_buttons_diff & mapping.button) {
                        digitalWrite(mapping.pin, buttons & mapping.button ? LOW : HIGH);
                    }
                }
            }
        }
    }

    Platform::loop();
}

int PicoPlatform::getFreeHeap() {
#ifdef ARDUINO_ARCH_RP2040
    return rp2040.getFreeHeap();
#else
    return 0;
#endif
}
