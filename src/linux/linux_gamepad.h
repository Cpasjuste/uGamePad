//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_GAMEPAD_H
#define U_GAMEPAD_LINUX_GAMEPAD_H

#include "gamepad.h"

namespace uGamePad {
    class LinuxGamePad : public GamePad {
    public:
        LinuxGamePad();

        void loop() override;
    };
}

#endif //U_GAMEPAD_LINUX_GAMEPAD_H
