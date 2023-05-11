//
// Created by cpasjuste on 05/04/23.
//

#ifndef U_GAMEPAD_LINUX_FS_H
#define U_GAMEPAD_LINUX_FS_H

namespace uGamePad {
    class LinuxFs : public Fs {
    public:
        LinuxFs();

        std::string getHomeDirectory() override { return "cmake-build-native/"; };

        bool writeFile(const std::string &path, const std::vector<uint8_t> &data) override;

        std::vector<uint8_t> readFile(const std::string &path) override;
    };
}

#endif //U_GAMEPAD_LINUX_FS_H
