//
// Created by cpasjuste on 29/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "pico_platform.h"

using namespace uGamePad;

PicoPlatform::PicoPlatform() = default;

void PicoPlatform::setup() {
    // if debugging on retail board, add a delay for serial init
#if defined(UGP_DEBUG) && !defined(UGP_DEV_BOARD)
    delay(2000);
#endif
#if defined(UGP_DEBUG)
    // setup serial debug
    Debug.setTX(GPIO_TX);
    Debug.setRX(GPIO_RX);
    Debug.begin();
#endif

    // init filesystem
    p_fs = new PicoFs();

    // init config
    p_config = new Config(p_fs);

    // init gfx
    p_gfx = new PicoGfx();

    // init gamepad
    p_pad = new PicoGamePad();

    // check for bootloader mode (hardware) button press
    if (p_pad->getButtons() & GamePad::Button::UP) {
        printf("PicoPlatform::setup: bootloader mode called\r\n");
        Utility::reboot(true);
        return;
    }

    // check for usb msc mode (hardware) button press
    if (p_pad->getButtons() & GamePad::Button::START) {
        printf("PicoPlatform::setup: usb msc mode called\r\n");
        p_fs->setUsbMode(Fs::UsbMode::Msc);
    } else {
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
    }

    // all done
    Platform::setup();
}

void PicoPlatform::loop() {
    if (p_fs->getUsbMode() == Fs::UsbMode::Host) {
        // handle usb host updates
        if (tuh_inited()) {
            tuh_task();
        } else {
            printf("error: tinyusb host service not inited...\r\n");
            while (true);
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
