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
        struct Profile {
            uint16_t vid;
            uint16_t pid;
            uint8_t slot;
        };

        explicit Config(Fs *fs);

        bool load();

        bool save();

        Device *loadDevice(uint16_t vid, uint16_t pid, uint8_t slot = 0);

        bool saveDevice(Device *device, uint8_t slot = 0);

        bool setSlot(uint16_t vid, uint16_t pid, uint8_t slot);

        uint8_t getSlot(uint16_t vid, uint16_t pid);

    private:
        Fs *p_fs;

        std::vector<Profile> m_profiles;

        Device *loadDevice(const std::string &path);
    };
} // uGamePad

#endif //U_GAMEPAD_CONFIG_H
