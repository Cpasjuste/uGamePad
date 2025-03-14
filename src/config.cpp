//
// Created by cpasjuste on 11/05/23.
//

#include <vector>
#include "config.h"

using namespace uGamePad;

Config::Config(Fs *fs) {
    p_fs = fs;
    p_fs->createDirectory(p_fs->getDeviceDirectory());

    // load configuration file
#if 0 // TESTING
    saveDevice(const_cast<Device *>(get_device_at(0)));
    auto dev = loadDevice(0x0079, 0x18d4);
    if (dev) {
        printf("device loaded: %s\r\n", dev->name);
    }
#endif
}

Device *Config::loadDevice(const uint16_t vid, const uint16_t pid) {
    char vendor[5], product[5];

    printf("Config::loadDevice: pid: 0x%04x, vid: 0x%04x\r\n", vid, pid);

    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    const std::string path = p_fs->getDeviceDirectory() + "/" + vendor + "-" + product + ".json";

    return loadDevice(path);
}

Device *Config::loadDevice(const std::string &path) {
    printf("Config::loadDevice: (%s)\r\n", path.c_str());

    if (Fs::fileExists(path)) {
        const Fs::File file{path};
        if (!file.isOpen()) {
            printf("Config::loadDevice: could not open file for reading... (%s)\r\n", path.c_str());
            return nullptr;
        }

        std::vector<uint8_t> buffer(file.getLength());
        const auto read = file.read(0, buffer.size(), reinterpret_cast<char *>(buffer.data()));
        if (read != buffer.size()) {
            printf("Config::loadDevice: could not read file... (%s)\r\n", path.c_str());
            return nullptr;
        }

        return Utility::deserializeDevice(&buffer);
    }

    return nullptr;
}

bool Config::saveDevice(Device *device) {
    if (!device) {
        printf("Config::saveDevice: filesystem not available...\r\n");
        return false;
    }

    std::vector<uint8_t> buffer(4096);
    const bool ret = Utility::serializeDevice(device, &buffer);
    if (!ret || buffer.empty()) {
        printf("Config::saveDevice: failed to serializeDevice device...\r\n");
        return false;
    }

    const std::string path =
            p_fs->getDeviceDirectory() + "/" + device->getVendor() + "-" + device->getProduct() + ".json";
    printf("Config::saveDevice: %s\r\n", path.c_str());

    const Fs::File file{path, Fs::File::OpenMode::Write};
    const int32_t wrote = file.write(0, buffer.size(), reinterpret_cast<const char *>(buffer.data()));

    return wrote == buffer.size();
}
