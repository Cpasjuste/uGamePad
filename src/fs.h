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

        enum UsbMode {
            Host,
            Msc
        };

        virtual bool isAvailable() { return m_available; };

        virtual std::string getHomeDirectory() { return "/"; };

        virtual std::string getDeviceDirectory() { return getHomeDirectory() + "devices"; };

        virtual bool writeFile(const std::string &path, const std::vector<uint8_t> &data) { return false; };

        virtual std::vector<uint8_t> readFile(const std::string &path) { return {}; };

        virtual void setUsbMode(UsbMode mode) { m_usb_mode = mode; };

        virtual UsbMode getUsbMode() { return m_usb_mode; };

        virtual DeviceInfo getDeviceInfo() { return {0, 0}; };

    protected:
        bool m_available = false;
        UsbMode m_usb_mode = UsbMode::Host;
    };
}

#endif //U_GAMEPAD_FS_H
