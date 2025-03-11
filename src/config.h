//
// Created by cpasjuste on 11/05/23.
//

#ifndef U_GAMEPAD_CONFIG_H
#define U_GAMEPAD_CONFIG_H

#include "fs.h"
#include "devices.h"

namespace uGamePad {
    class Config {
    public:
        explicit Config(Fs *fs);

        Device *loadDevice(uint16_t vid, uint16_t pid);

        bool saveDevice(Device *device);

    private:
        Fs *p_fs;

        Device *loadDevice(const std::string &path);
    };
} // uGamePad

#endif //U_GAMEPAD_CONFIG_H
