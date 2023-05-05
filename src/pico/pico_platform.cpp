//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "clock.h"
#include "pico_platform.h"

using namespace uGamePad;

PicoPlatform::PicoPlatform() = default;

void uGamePad::PicoPlatform::setup() {
    // setup serial debug
    Debug.setTX(PIN_TX);
    Debug.setRX(PIN_RX);
    Debug.begin();

    // init filesystem
    p_fs = new PicoFs();

    // init hardware buttons
    pinMode(GPIO_BUTTON_UP, INPUT_PULLUP);
    pinMode(GPIO_BUTTON_DOWN, INPUT_PULLUP);
    pinMode(GPIO_BUTTON_ENTER, INPUT_PULLUP);

    // check for bootloader mode button press
    if (!digitalRead(GPIO_BUTTON_UP)) {
        printf("GPIO_BUTTON_UP: bootloader mode called\r\n");
        Utility::reboot(true);
        return;
    }

    // check for usb msc button press
    if (!digitalRead(GPIO_BUTTON_ENTER)) {
        printf("GPIO_BUTTON_ENTER: usb msc mode called\r\n");
        p_fs->share();
        return;
    }

    // init gfx
    p_gfx = new PicoGfx();

    // init gamepad
    p_pad = new PicoGamePad();

    // init usb stack
    if (!tusb_init()) {
        printf("tusb_init failed...\r\n");
        while (true);
    }

    // init usb host stack
    if (!tuh_init(0)) {
        printf("tuh_init failed...\r\n");
        while (true);
    }

    // all done
    Platform::setup();
}

void PicoPlatform::loop() {
    // if flash filesystem is in usb msc mode (shared), just return
    if (p_fs->isShared()) return;

    // handle usb host updates
    if (tuh_inited()) {
        tuh_task();
    } else {
        printf("error: tinyusb host service not inited...\r\n");
        while (true);
    }

    // handle gamepad states
    if (p_ui && !p_ui->isActive()) {
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
