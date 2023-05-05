//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_PICO_FS_H
#define U_GAMEPAD_PICO_FS_H

#include "SdFat.h"

namespace uGamePad {
    class PicoFs : public Fs {
    public:
        PicoFs();

        Device *load(uint16_t vid, uint16_t pid) override;

        Device *load(const std::string &filename) override;

        bool save(Device *device) override;

        DeviceInfo getDeviceInfo() override;

        void share() override;
    };
}

#endif //U_GAMEPAD_PICO_FS_H
