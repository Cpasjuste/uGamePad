//
// Created by cpasjuste on 11/05/23.
//

#include <vector>
#include "config.h"

using namespace uGamePad;

Config::Config(Fs *fs) {
    p_fs = fs;
    p_fs->createDirectory(p_fs->getDeviceDirectory());

    // setup options
    //std::vector<std::string> modes;
    //for (const auto &out: getPlatform()->getPad()->getOutputModes()) {
    //    modes.push_back(out.name);
    //}
    /*
    m_options.push_back({OptionId::OutputMode, OptionType::Option, "OUTPUT MODE", {}});
    m_options.push_back({OptionId::ButtonsRemap, OptionType::SubMenu, "BUTTONS REMAP", {"GO"}});
    m_options.push_back({OptionId::AutoFire, OptionType::Option, "AUTO FIRE", {"OFF", "ON"}});
    m_options.push_back({OptionId::GamePadTest, OptionType::SubMenu, "GAMEPAD TEST", {"GO"}});
    m_options.push_back({OptionId::Exit, OptionType::SubMenu, "EXIT", {"GO"}});
    */

    // load configuration file

#if 0 // TESTING
    saveDevice(const_cast<Device *>(get_device_at(0)));
    auto dev = loadDevice(0x0079, 0x18d4);
    if (dev) {
        printf("device loaded: %s\r\n", dev->name);
    }
#endif
}

Device *Config::loadDevice(uint16_t vid, uint16_t pid) {
    char vendor[5], product[5];
    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    std::string path = p_fs->getDeviceDirectory() + "/" + vendor + "-" + product + ".json";
    return loadDevice(path);
}

Device *Config::loadDevice(const std::string &path) {
    if (Fs::fileExists(path)) {
        Fs::File file{path};
        if (!file.isOpen()) {
            printf("Config::loadDevice: could not open file for reading... (%s)\r\n", path.c_str());
            return nullptr;
        }

        std::vector<uint8_t> buffer(file.getLength());
        int32_t read = file.read(0, buffer.size(), reinterpret_cast<char *>(buffer.data()));
        if (read != buffer.size()) {
            printf("Config::loadDevice: could not read file... (%s)\r\n", path.c_str());
            return nullptr;
        }

        return Utility::deserialize(&buffer);
    }

    return nullptr;
}

bool Config::saveDevice(Device *device) {
    if (!device) {
        printf("Config::saveDevice: filesystem not available...\r\n");
        return false;
    }

    std::vector<uint8_t> buffer(4096);
    bool ret = Utility::serialize(device, &buffer);
    if (!ret || buffer.empty()) {
        printf("Config::saveDevice: failed to serialize device...\r\n");
        return false;
    }

    std::string path = p_fs->getDeviceDirectory() + "/" + device->getVendor() + "-" + device->getProduct() + ".json";
    Fs::File file{path, Fs::File::OpenMode::Write};
    int32_t wrote = file.write(0, buffer.size(), reinterpret_cast<const char *>(buffer.data()));
    return wrote == buffer.size();
}
