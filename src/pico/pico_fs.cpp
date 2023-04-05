//
// Created by cpasjuste on 05/04/23.
//

#include <LittleFS.h>
#include "pico_platform.h"
#include "pico_fs.h"

using namespace uGamePad;

PicoFs::PicoFs() : Fs() {
    LittleFSConfig cfg;
    cfg.setAutoFormat(true);
    LittleFS.setConfig(cfg);
    if (!LittleFS.begin()) {
        printf("err: filesystem init failed\n");
        return;
    }

    Fs::DeviceInfo info = PicoFs::getDeviceInfo();
    printf("filesystem size: %i, usage: %i\r\n", info.totalBytes, info.usedBytes);

    // testing
    PicoFs::save(const_cast<Device *>(get_device_at(0)));
}

Fs::DeviceInfo PicoFs::getDeviceInfo() {
    FSInfo64 info{};
    LittleFS.info64(info);
    return {info.totalBytes, info.usedBytes};
}

bool PicoFs::save(Device *device) {
    if (!device) return false;
    std::string name = std::to_string(device->idVendor) + "x" + std::to_string(device->idProduct) + ".cfg";
    std::string path = "/" + name;
    File f = LittleFS.open(path.c_str(), "w");
    if (!f) {
        printf("err: could not open %s for writing...\r\n", path.c_str());
        return false;
    }

    // TODO

    return true;
}
