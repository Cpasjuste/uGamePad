//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_PLATFORM_H
#define U_GAMEPAD_LINUX_PLATFORM_H

#include "platform.h"

namespace uGamePad {
    class LinuxPlatform : public Platform {
    public:
        LinuxPlatform();

        void loop() override;
    };
}

#endif //U_GAMEPAD_LINUX_PLATFORM_H
