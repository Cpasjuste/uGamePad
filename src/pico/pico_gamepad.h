//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_GAMEPAD_H
#define U_GAMEPAD_PICO_GAMEPAD_H

#include "gamepad.h"

namespace uGamePad {
    class PicoGamePad : public GamePad {
    public:
        bool update(const uint8_t *report, uint16_t len) override;

        PinMapping *getPinMapping() override;

        void setLed(uint8_t type) override;
    };
}

#endif //U_GAMEPAD_PICO_GAMEPAD_H
