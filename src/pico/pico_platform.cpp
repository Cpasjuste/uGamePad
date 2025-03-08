//
// Created by cpasjuste on 29/03/23.
//

#include <pico/stdio.h>
#include <device/usbd.h>

#include "main.h"
#include "pico_platform.h"
#include "pico_gamepad.h"
#include "pico_fs.h"
#include "pico_hid.h"

using namespace uGamePad;

PicoPlatform::PicoPlatform() = default;

void PicoPlatform::setup() {
    stdio_init_all();

    // if debugging on retail board, add a delay for serial init
#if defined(UGP_DEBUG) && !defined(UGP_DEV_BOARD)
    delay(2000);
#endif

    // init filesystem
    p_fs = new PicoFs();

    // init config
    p_config = new Config(p_fs);

    // init gamepad
    p_pad = new PicoGamePad();

    // linux hid api
    p_hid = new PicoHid();

    // init gfx
    p_gfx = new PicoGfx();

    // check for bootloader mode (hardware) button press
    auto hardwareButtons = PicoGamePad::getHardwareButtons();
    if (hardwareButtons & GamePad::Button::UP) {
        printf("PicoPlatform::setup: bootloader mode called\r\n");
        Utility::reboot(true);
        return;
    }

    // check for usb msc mode (hardware) button press
    if (hardwareButtons & GamePad::Button::DOWN) {
        printf("PicoPlatform::setup: usb msc mode called\r\n");
        p_fs->setUsbMode(Fs::UsbMode::Msc);
    } else {
        p_fs->setUsbMode(Fs::UsbMode::Host);
    }

    stdio_flush();

    // all done
    Platform::setup();
}

void PicoPlatform::loop() {
    if (p_fs->getUsbMode() == Fs::UsbMode::Msc) {
        // handle usb device (msc) updates
        if (tud_inited()) {
            tud_task();
        } else {
            printf("error: tinyusb device service not started...\r\n");
            return;
        }
    }

    Platform::loop();
}

int PicoPlatform::getFreeHeap() {
    return 0;
}

// https://github.com/raspberrypi/pico-sdk/issues/1768
extern "C" {
int _getentropy(void *buffer, size_t length) {
    return -1;
}
}
