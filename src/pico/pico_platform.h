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
