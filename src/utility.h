//
// Created by cpasjuste on 25/03/23.
//

#ifndef U_GAMEPAD_UTILITY_H
#define U_GAMEPAD_UTILITY_H

#include <string>

namespace uGamePad {
    class Utility {
    public:
        static void reboot(bool bootloader = false);

        static std::string toString(uint32_t button);
    };
}

#endif //U_GAMEPAD_UTILITY_H
