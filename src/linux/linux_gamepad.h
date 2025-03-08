//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_GAMEPAD_H
#define U_GAMEPAD_LINUX_GAMEPAD_H

#include "gamepad.h"

namespace uGamePad {
    class LinuxGamePad final : public GamePad {
    public:
        LinuxGamePad();

        ~LinuxGamePad() override = default;

        void loop() override;
    };
}

#endif //U_GAMEPAD_LINUX_GAMEPAD_H
