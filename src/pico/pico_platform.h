//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#warning "TODO: remove static def"
#define UGP_DEV_BOARD

#if defined(PICO_BUILD)
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
#define GPIO_TX             0
#define GPIO_RX             1
// hardware buttons
#define GPIO_BUTTON_UP      17
#define GPIO_BUTTON_DOWN    20
#define GPIO_BUTTON_ENTER   21
#define GPIO_OLED_SDA       4
#define GPIO_OLED_SCL       5
// output pins
#define GPIO_BTN_B1         15
#define GPIO_BTN_B2         10
#define GPIO_BTN_B3         14
#define GPIO_BTN_B4         11
#define GPIO_BTN_B5         27
#define GPIO_BTN_B6         28
#define GPIO_BTN_SELECT     26
#define GPIO_BTN_START      22
#define GPIO_BTN_UP         8
#define GPIO_BTN_DOWN       7
#define GPIO_BTN_LEFT       16
#define GPIO_BTN_RIGHT      9
// nes pins
#define GPIO_NES_LATCH      2
#define GPIO_NES_CLOCK      3
#define GPIO_NES_DATA       4
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
