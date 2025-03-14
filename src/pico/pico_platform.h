//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#if defined(PICO_BUILD)
#if PICO_RP2350
// RP2350 testing
#define GPIO_TX             0
#define GPIO_RX             1
// hardware buttons
#define GPIO_HW_BTN_UP      INPUT_DUMMY
#define GPIO_HW_BTN_DOWN    INPUT_DUMMY
#define GPIO_HW_BTN_ENTER   INPUT_DUMMY
#define GPIO_OLED_SDA       4
#define GPIO_OLED_SCL       5
// output pins
#define GPIO_BTN_B1         INPUT_DUMMY
#define GPIO_BTN_B2         INPUT_DUMMY
#define GPIO_BTN_B3         INPUT_DUMMY
#define GPIO_BTN_B4         INPUT_DUMMY
#define GPIO_BTN_B5         INPUT_DUMMY
#define GPIO_BTN_B6         INPUT_DUMMY
#define GPIO_BTN_SELECT     INPUT_DUMMY
#define GPIO_BTN_START      INPUT_DUMMY
#define GPIO_BTN_UP         INPUT_DUMMY
#define GPIO_BTN_DOWN       INPUT_DUMMY
#define GPIO_BTN_LEFT       INPUT_DUMMY
#define GPIO_BTN_RIGHT      INPUT_DUMMY
#elif defined(UGP_DEV_BOARD)
// "uGamePad_v1.1_pico-usb-a" dev board
#define GPIO_TX             0
#define GPIO_RX             1
// hardware buttons
#define GPIO_HW_BTN_UP      17
#define GPIO_HW_BTN_DOWN    20
#define GPIO_HW_BTN_ENTER   21
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
#else
// uGamePad retail board v1.1
// use pins 0 and 1 for tx/rx (buttons 5/6)
#define GPIO_TX             0  // vga pin 10
#define GPIO_RX             1  // vga pin 2
#define GPIO_HW_BTN_UP      14
#define GPIO_HW_BTN_DOWN    15
#define GPIO_HW_BTN_ENTER   26
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
