//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_GAMEPAD_H
#define U_GAMEPAD_PICO_GAMEPAD_H

// nes pins
#define NES_LATCH   D2
#define NES_CLOCK   D3
#define NES_DATA    D4

namespace uGamePad {
    class PicoGamePad : public GamePad {
    public:
        PicoGamePad();

        bool report(const uint8_t *report, uint16_t len);

        void setOutputMode(const Mode &mode) override;

        void loop() override;

    private:
        static void onLatchRising();

        static void onClockFalling();

        uint16_t m_buttons_old{}, m_buttons_diff{};
    };
}

#endif //U_GAMEPAD_PICO_GAMEPAD_H
