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

        virtual std::string getHome() { return "/"; };

        virtual std::string getDevicesDirectory() { return getHome() + "devices"; };

        virtual Device *load(uint16_t vid, uint16_t pid) { return nullptr; };

        virtual bool save(Device *device) { return false; };

        virtual void setUsbMode(UsbMode mode) { m_usb_mode = mode; };

        virtual UsbMode getUsbMode() { return m_usb_mode; };

        virtual DeviceInfo getDeviceInfo() { return {0, 0}; };

    protected:
        virtual Device *load(const std::string &filename) { return nullptr; };
        bool m_available = false;
        UsbMode m_usb_mode = UsbMode::Host;
    };
}

#endif //U_GAMEPAD_FS_H
