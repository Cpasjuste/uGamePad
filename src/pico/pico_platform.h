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

namespace uGamePad {
    class PicoPlatform : public Platform {
    public:
        PicoPlatform();

        void setup() override;

        void loop() override;

        int getFreeHeap() override;

    private:
        uint16_t buttons, buttons_old, buttons_diff;
    };
}

#endif //U_GAMEPAD_PICO_PLATFORM_H
