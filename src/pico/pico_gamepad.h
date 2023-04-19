//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_GAMEPAD_H
#define U_GAMEPAD_PICO_GAMEPAD_H

// nes testing
#if ARDUINO_SEEED_XIAO_RP2040
#define NES_LATCH   D10
#define NES_CLOCK   D9
#define NES_DATA    D8
#else
#define NES_LATCH   D21
#define NES_CLOCK   D20
#define NES_DATA    D19
#endif

namespace uGamePad {
    class PicoGamePad : public GamePad {
    public:
        PicoGamePad();

        bool update(const uint8_t *report, uint16_t len) override;

        PinMapping *getPinMapping() override;

        void setMode(const Mode &mode) override;
    };
}

#endif //U_GAMEPAD_PICO_GAMEPAD_H
