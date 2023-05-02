//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_FS_H
#define U_GAMEPAD_FS_H

#include <string>
#include <cstdint>
#include "devices.h"

namespace uGamePad {
    class Fs {
    public:
        struct DeviceInfo {
            uint64_t totalBytes;
            uint64_t usedBytes;
        };

        Fs();

        virtual bool isAvailable() { return m_available; };

        virtual std::string getHome() { return "/"; };

        virtual Device *load(uint16_t vid, uint16_t pid) { return nullptr; };

        virtual Device *load(const std::string &path) { return nullptr; };

        virtual bool save(Device *device) { return false; };

        virtual void share() {};

        virtual bool isShared() { return m_shared; };

        virtual DeviceInfo getDeviceInfo() { return {0, 0}; };

    protected:
        bool m_available = false;
        bool m_shared = false;
    };
}

#endif //U_GAMEPAD_FS_H
