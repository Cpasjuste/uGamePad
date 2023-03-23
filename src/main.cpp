/*
 Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Arduino.h>
#include "led.h"
#include "xbox_usb.h"

// On SAMD boards where the native USB port is also the serial console, use
// Serial1 for the serial console. This applies to all SAMD boards except for
// Arduino Zero and M0 boards.
#if (USB_VID == 0x2341 && defined(ARDUINO_SAMD_ZERO)) || (USB_VID == 0x2a03 && defined(ARDUINO_SAM_ZERO))
#define SerialDebug SERIAL_PORT_MONITOR
#else
#define SerialDebug Serial1
#endif

GameController *gameController;

void setup() {
    // serial debug
    SerialDebug.begin(115200);

    // led
    pinMode(LED_PIN, OUTPUT);

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

    gameController = new GameController();
    SerialDebug.println("\r\nusbh-gamepad started");
}

void loop() {
    Led::Update();
    gameController->update();

    if (gameController->isConnected()) {
        if (gameController->getButtonClick(A)) {
            SerialDebug.println("rebooting...");
            SerialDebug.flush();
            delay(1000);
            // reset to bootloader
            //*((volatile uint32_t *) (HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4)) = 0xf01669ef;
            // normal reset
            NVIC_SystemReset();
        }
    }

    delay(1);
}
