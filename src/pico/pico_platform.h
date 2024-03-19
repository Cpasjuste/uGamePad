//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#include "pico_gamepad.h"
#include "pico_gfx.h"

#if defined(PICO_BUILD)
#if defined(UGP_DEV_BOARD)
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

// uGamePad retail board
// use pins 0 and 1 for tx/rx (buttons 5/6)
#define GPIO_TX             0  // vga pin 10
#define GPIO_RX             1  // vga pin 2
#define GPIO_BUTTON_UP      14
#define GPIO_BUTTON_DOWN    15
#define GPIO_BUTTON_ENTER   26
#define GPIO_OLED_SDA       12
#define GPIO_OLED_SCL       13
// output pins
#define GPIO_BTN_B1         3
#define GPIO_BTN_B2         4
#define GPIO_BTN_B3         5
#define GPIO_BTN_B4         9
#define GPIO_BTN_B5         0
#define GPIO_BTN_B6         1
#define GPIO_BTN_SELECT     8
#define GPIO_BTN_START      6
#define GPIO_BTN_UP         10
#define GPIO_BTN_DOWN       11
#define GPIO_BTN_LEFT       2
#define GPIO_BTN_RIGHT      7
// nes pins
#define GPIO_NES_LATCH      2
#define GPIO_NES_CLOCK      3
#define GPIO_NES_DATA       4
#elif defined(RETROPICO_BOARD)
// uGamePad retail board
// use pins 0 and 1 for tx/rx (buttons 5/6)
#define GPIO_TX             16  // vga pin 10
#define GPIO_RX             17  // vga pin 2
#define GPIO_BUTTON_UP      (8)
#define GPIO_BUTTON_DOWN    (9)
#define GPIO_BUTTON_ENTER   (15)
// output pins
#define GPIO_BTN_B1         UINT8_MAX
#define GPIO_BTN_B2         UINT8_MAX
#define GPIO_BTN_B3         UINT8_MAX
#define GPIO_BTN_B4         UINT8_MAX
#define GPIO_BTN_B5         UINT8_MAX
#define GPIO_BTN_B6         UINT8_MAX
#define GPIO_BTN_SELECT     UINT8_MAX
#define GPIO_BTN_START      UINT8_MAX
#define GPIO_BTN_UP         UINT8_MAX
#define GPIO_BTN_DOWN       UINT8_MAX
#define GPIO_BTN_LEFT       UINT8_MAX
#define GPIO_BTN_RIGHT      UINT8_MAX
#else
// uGamePad retail board
// use pins 0 and 1 for tx/rx (bouttons 5/6)
#define GPIO_TX             0  // vga pin 10
#define GPIO_RX             1  // vga pin 2
#define GPIO_BUTTON_UP      14
#define GPIO_BUTTON_DOWN    15
#define GPIO_BUTTON_ENTER   26
#define GPIO_OLED_SDA       12
#define GPIO_OLED_SCL       13
// output pins
#define GPIO_BTN_B1         3
#define GPIO_BTN_B2         4
#define GPIO_BTN_B3         5
#define GPIO_BTN_B4         9
#define GPIO_BTN_B5         0
#define GPIO_BTN_B6         1
#define GPIO_BTN_SELECT     8
#define GPIO_BTN_START      6
#define GPIO_BTN_UP         10
#define GPIO_BTN_DOWN       11
#define GPIO_BTN_LEFT       2
#define GPIO_BTN_RIGHT      7
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
