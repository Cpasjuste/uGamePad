//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_PLATFORM_H
#define U_GAMEPAD_PICO_PLATFORM_H

#include <Arduino.h>
#include "platform.h"

#ifdef NATIVE
#else
#define Debug Serial1
#undef printf
#define printf Serial1.printf
#endif

namespace uGamePad {
    class PicoPlatform : public Platform {
    public:
        PicoPlatform();

        void loop() override;
    };
}

#endif //U_GAMEPAD_PICO_PLATFORM_H
