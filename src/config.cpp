//
// Created by cpasjuste on 11/05/23.
//

#include <vector>
#include "main.h"
#include "config.h"
#include "utility/json.h"

using namespace uGamePad;

Config::Config(Fs *fs) {
    p_fs = fs;
    p_fs->createDirectory(p_fs->getDeviceDirectory());

#if 1 // TESTING
    save(const_cast<Device *>(get_device_at(0)));
    auto dev = load(0x0079, 0x18d4);
    if (dev) {
        printf("device loaded: %s\r\n", dev->name);
    }
#endif
}

Device *Config::load(uint16_t vid, uint16_t pid) {
    char vendor[5], product[5];
    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    std::string path = p_fs->getDeviceDirectory() + "/" + vendor + "-" + product + ".json";
    return load(path);
}

Device *Config::load(const std::string &path) {
    std::vector<uint8_t> buffer = p_fs->readFile(path);
    if (buffer.empty()) {
        printf("Config::load: could not read file...\r\n");
        return nullptr;
    }

    return Json::getDevice(buffer);
}

bool Config::save(Device *device) {
    if (!device) {
        printf("Config::save: filesystem not available...\r\n");
        return false;
    }

    std::vector<uint8_t> buffer = Json::getDevice(device);
    if (buffer.empty()) {
        printf("Config::save: failed to serialize device...\r\n");
        return false;
    }

    std::string path = p_fs->getDeviceDirectory() + "/" + device->getVendor() + "-" + device->getProduct() + ".json";
    return p_fs->writeFile(path, buffer);
}
