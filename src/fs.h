//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_FS_H
#define U_GAMEPAD_FS_H

#include <cstdint>
#include "devices.h"

namespace uGamePad {
    class Fs {
    public:
        struct DeviceInfo {
            std::size_t totalBytes;
            std::size_t usedBytes;
        };

        Fs();

        virtual DeviceInfo getDeviceInfo() { return {0, 0}; };

        virtual bool save(Device *device) { return false; };
    };
}

#endif //U_GAMEPAD_FS_H
