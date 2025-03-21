//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_PLATFORM_H
#define U_GAMEPAD_LINUX_PLATFORM_H

#include <unistd.h>
#include "linux_hid.h"

#define delay(x) usleep(x * 1000)

namespace uGamePad {
    class LinuxPlatform : public Platform {
    public:
        void setup() override;
    };
}

#endif //U_GAMEPAD_LINUX_PLATFORM_H
