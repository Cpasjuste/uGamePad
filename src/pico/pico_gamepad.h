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

        bool update(const uint8_t *report, uint16_t len) override;

        Output *getOutputMode() override;

    private:
        static void onLatchRising();

        static void onClockFalling();
    };
}

#endif //U_GAMEPAD_PICO_GAMEPAD_H
