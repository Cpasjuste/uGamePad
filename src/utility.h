//
// Created by cpasjuste on 25/03/23.
//

#ifndef U_GAMEPAD_UTILITY_H
#define U_GAMEPAD_UTILITY_H

#include <cstdint>
#include <cmath>
#include <string>

namespace uGamePad {
    class Utility {
    public:
        enum Color {
            Black = 0,
            White = 1,
            Inverse = 2
        };

        struct Vector2f {
            float x;
            float y;
        };

        struct Vector2i {
            int16_t x;
            int16_t y;
        };

        struct Vector4i {
            int16_t x;
            int16_t y;
            int16_t w;
            int16_t h;
        };

        struct Vector6f {
            Vector2f points[3];
        };

        struct Line {
            Vector2f start;
            Vector2f end;
        };

        static void reboot(bool bootloader = false);

        static std::string toString(uint32_t button);
    };
}

#endif //U_GAMEPAD_UTILITY_H
