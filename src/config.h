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

        bool save(Device *device);

        Device *load(uint16_t vid, uint16_t pid);

    private:
        Fs *p_fs;

        Device *load(const std::string &path);
    };

} // uGamePad

#endif //U_GAMEPAD_CONFIG_H
