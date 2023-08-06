//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#include <Arduino.h>

#ifndef NATIVE
#define Debug Serial1
#undef printf
#define printf Serial1.printf
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO)
#if !defined(UGP_DEV_BOARD)
// uGamePad retail board
// use pins 0 and 1 for tx/rx (bouttons 5/6)
#define GPIO_TX             D0  // vga pin 10
#define GPIO_RX             D1  // vga pin 2
#define GPIO_BUTTON_UP      D14
#define GPIO_BUTTON_DOWN    D15
#define GPIO_BUTTON_ENTER   D26
#define GPIO_OLED_SDA       D12
#define GPIO_OLED_SCL       D13
// output pins
#define GPIO_BTN_B1         D3
#define GPIO_BTN_B2         D4
#define GPIO_BTN_B3         D5
#define GPIO_BTN_B4         D9
#define GPIO_BTN_B5         D0
#define GPIO_BTN_B6         D1
#define GPIO_BTN_SELECT     D8
#define GPIO_BTN_START      D6
#define GPIO_BTN_UP         D10
#define GPIO_BTN_DOWN       D11
#define GPIO_BTN_LEFT       D2
#define GPIO_BTN_RIGHT      D7
// nes pins
#define GPIO_NES_LATCH      D2
#define GPIO_NES_CLOCK      D3
#define GPIO_NES_DATA       D4
#else
// uGamePad pico dev board
// debug pins
#define GPIO_TX             D0
#define GPIO_RX             D1
// hardware buttons
#define GPIO_BUTTON_UP      D17
#define GPIO_BUTTON_DOWN    D20
#define GPIO_BUTTON_ENTER   D21
#define GPIO_OLED_SDA       D4
#define GPIO_OLED_SCL       D5
// output pins
#define GPIO_BTN_B1         D15
#define GPIO_BTN_B2         D10
#define GPIO_BTN_B3         D14
#define GPIO_BTN_B4         D11
#define GPIO_BTN_B5         D27
#define GPIO_BTN_B6         D28
#define GPIO_BTN_SELECT     D26
#define GPIO_BTN_START      D22
#define GPIO_BTN_UP         D8
#define GPIO_BTN_DOWN       D7
#define GPIO_BTN_LEFT       D16
#define GPIO_BTN_RIGHT      D9
// nes pins
#define GPIO_NES_LATCH      D2
#define GPIO_NES_CLOCK      D3
#define GPIO_NES_DATA       D4
#endif
#endif

namespace uGamePad {
    class PicoPlatform : public Platform {
    public:
        PicoPlatform();

        void setup() override;

        void loop() override;

        int getFreeHeap() override;
    };
}

#endif //U_GAMEPAD_PICO_PLATFORM_H
