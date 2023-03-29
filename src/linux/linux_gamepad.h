//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_GAMEPAD_H
#define U_GAMEPAD_LINUX_GAMEPAD_H

#include "gamepad.h"

namespace uGamePad {
    class LinuxGamePad : public GamePad {
    public:
        bool update(const uint8_t *report, uint16_t len) override;
    };
}

#endif //U_GAMEPAD_LINUX_GAMEPAD_H
