//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_PICO_FS_H
#define U_GAMEPAD_PICO_FS_H

#include "fs.h"

namespace uGamePad {
    class PicoFs : public Fs {
    public:
        PicoFs();

        bool save(Device *device) override;

        DeviceInfo getDeviceInfo() override;
    };
}

#endif //U_GAMEPAD_PICO_FS_H
