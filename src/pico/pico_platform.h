//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#ifndef NATIVE
#define Debug Serial1
#undef printf
#define printf Serial1.printf
#endif

//#define DEBUG_RP2040_ZERO 1

#if defined(ARDUINO_RASPBERRY_PI_PICO)
#if defined(DEBUG_RP2040_ZERO)
// uGamePad retail board
// use pins 0 and 1 for tx/rx (bouttons 5/6)
#define PIN_TX  D0  // vga pin 10
#define PIN_RX  D1  // vga pin 2
#else
// uGamePad pico dev board
#define PIN_TX  D16
#define PIN_RX  D17
#endif
#endif

#define GPIO_BUTTON_UP      D14
#define GPIO_BUTTON_DOWN    D15
#define GPIO_BUTTON_ENTER   D26

namespace uGamePad {
    class PicoPlatform : public Platform {
    public:
        PicoPlatform();

        void setup() override;

        void loop() override;

        int getFreeHeap() override;

    private:
        uint16_t m_buttons_old{}, m_buttons_diff{};
    };
}

#endif //U_GAMEPAD_PICO_PLATFORM_H
