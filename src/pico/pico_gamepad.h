//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_GAMEPAD_H
#define U_GAMEPAD_PICO_GAMEPAD_H

namespace uGamePad {
    class PicoGamePad : public GamePad {
    public:
        PicoGamePad();

        bool onHidReport(const uint8_t *report, uint16_t len) override;

        void setOutputMode(const Mode &mode) override;

    private:
#ifndef TODO_NES_SNES_MD_CABLES
        static void onLatchRising();

        static void onClockFalling();
#endif
        uint16_t m_buttons_old{}, m_buttons_diff{};
    };
}

#endif //U_GAMEPAD_PICO_GAMEPAD_H
