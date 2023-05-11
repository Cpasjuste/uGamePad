//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_PICO_FS_H
#define U_GAMEPAD_PICO_FS_H

namespace uGamePad {
    class PicoFs : public Fs {
    public:
        PicoFs();

        bool writeFile(const std::string &path, const std::vector<uint8_t> &data) override;

        std::vector<uint8_t> readFile(const std::string &path) override;

        DeviceInfo getDeviceInfo() override;

        void setUsbMode(UsbMode mode) override;
    };
}

#endif //U_GAMEPAD_PICO_FS_H
