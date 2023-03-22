/*
 Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Arduino.h>
#include "XBOXUSB.h"
#include "PS3USB.h"

// On SAMD boards where the native USB port is also the serial console, use
// Serial1 for the serial console. This applies to all SAMD boards except for
// Arduino Zero and M0 boards.
#if (USB_VID == 0x2341 && defined(ARDUINO_SAMD_ZERO)) || (USB_VID == 0x2a03 && defined(ARDUINO_SAM_ZERO))
#define SerialDebug SERIAL_PORT_MONITOR
#else
#define SerialDebug Serial1
#endif

USBHost UsbH;

XBOXUSB Xbox(&UsbH);
PS3USB Ps3(&UsbH);
USBDeviceConfig *controller;

void setup() {
    // led
    //pinMode(LED_BUILTIN, OUTPUT);

    /*
    // init output pins
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    //pinMode(A6, OUTPUT); // TX UART debug
    //pinMode(A7, OUTPUT); // RX UART debug
    pinMode(A8, OUTPUT);
    pinMode(A9, OUTPUT);
    pinMode(A10, OUTPUT);
    // SWCLK
    // pinMode(32, INPUT); // patch to use the SWCLK pin as output
    pinMode(32, OUTPUT);
    */

    SerialDebug.begin(115200);
    if (UsbH.Init()) {
        SerialDebug.println("\r\nusb host library error...");
        while (1); //halt
    }
    SerialDebug.println("\r\nusb host library started");
}

void loop() {
    UsbH.Task();

    if (Xbox.Xbox360Connected) {
        controller = &Xbox;
        if (Xbox.getButtonClick(A)) {
            SerialDebug.println("rebooting ");
            SerialDebug.flush();
            delay(1000);
            // reset to bootloader
            *((volatile uint32_t *) (HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4)) = 0xf01669ef;
            NVIC_SystemReset();
        }
    } else if (Ps3.PS3Connected) {
        controller = &Ps3;
    } else {
        controller = nullptr;
    }

    if (controller) {
        //if(controller.)
    }

    delay(1);
}
