//
// Created by cpasjuste on 11/05/23.
//

#include <vector>
#include "config.h"

#undef ARDUINO
#define ARDUINOJSON_ENABLE_STD_STREAM 1
#include "ArduinoJson-v7.3.1.h"

using namespace uGamePad;

Config::Config(Fs *fs) {
    p_fs = fs;
    p_fs->createDirectory(p_fs->getDevicesDirectory());

    if (!load()) {
        save();
    }

#ifndef NDEBUG
    const auto files = Fs::getDir(p_fs->getDevicesDirectory());
    printf("Config: listing devices directory (%s)\r\n", p_fs->getDevicesDirectory().c_str());
    for (const auto &file: files) {
        printf("\t- %s\r\n", file.name.c_str());
    }
#endif
}

bool Config::load() {
    const std::string path = p_fs->getRootDirectory() + "config.json";
    printf("Config::load: %s\r\n", path.c_str());

    if (!Fs::fileExists(path)) {
        printf("Config::load: file not found (%s)\r\n", path.c_str());
        return false;
    }

    const Fs::File file{path};
    if (!file.isOpen()) {
        printf("Config::load: could not open file for reading... (%s)\r\n", path.c_str());
        return false;
    }

    std::vector<uint8_t> buffer(file.getLength());
    const auto read = file.read(0, buffer.size(), reinterpret_cast<char *>(buffer.data()));
    if (read != buffer.size()) {
        printf("Config::load: could not read file... (%s)\r\n", path.c_str());
        return false;
    }

    JsonDocument doc;
    const auto err = deserializeJson(doc, buffer.data(), buffer.size());
    if (err) {
        printf("Config::load: failed to deserialize buffer: %s\r\n", err.c_str());
        return false;
    }

    m_profiles.clear();

    const JsonArray profiles = doc["profiles"];
    for (const auto &p: profiles) {
        const Profile profile{.vid = p["vid"], .pid = p["pid"], .slot = p["slot"]};
        m_profiles.push_back(profile);
#ifndef NDEBUG
        printf("Config::load: found profile for %4hx:%4hx slot: %02i\r\n",
               profile.vid, profile.pid, profile.slot);
#endif
    }

    return true;
}

bool Config::save() {
    const std::string path = p_fs->getRootDirectory() + "config.json";
    std::vector<uint8_t> buffer(4096);
    JsonDocument doc;

    printf("Config::save: %s\r\n", path.c_str());

    const auto array = doc["profiles"].to<JsonArray>();
    for (const auto &p: m_profiles) {
        auto item = array.add<JsonObject>();
        item["vid"] = p.vid;
        item["pid"] = p.pid;
        item["slot"] = p.slot;
    }

    const size_t len = serializeJsonPretty(doc, buffer.data(), buffer.size());
    if (len == 0) {
        printf("Json::getDevice: failed to serializeDevice device...\r\n");
        return {};
    }

    // resize buffer for writing
    buffer.resize(len);

    const Fs::File file{path, Fs::File::OpenMode::Write};
    const int32_t wrote = file.write(0, buffer.size(), reinterpret_cast<const char *>(buffer.data()));

    return wrote == buffer.size();
}

Device *Config::loadDevice(const uint16_t vid, const uint16_t pid, uint8_t s) {
    char vendor[5], product[5], slot[5];

    printf("Config::loadDevice: pid: 0x%04x, vid: 0x%04x\r\n", vid, pid);

    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    snprintf(slot, 5, "%02i", s);

    const std::string path = p_fs->getDevicesDirectory() + "/" + vendor + "-" + product
                             + "-" + slot + ".json";

    return loadDevice(path);
}

Device *Config::loadDevice(const std::string &path) {
    printf("Config::loadDevice: %s\r\n", path.c_str());

    if (!Fs::fileExists(path)) {
        printf("Config::loadDevice: file not found... (%s)\r\n", path.c_str());
        return nullptr;
    }

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

bool Config::saveDevice(Device *device, uint8_t s) {
    if (!device) {
        printf("Config::saveDevice: device is null...\r\n");
        return false;
    }

    std::vector<uint8_t> buffer(4096);
    const bool ret = Utility::serializeDevice(device, &buffer);
    if (!ret || buffer.empty()) {
        printf("Config::saveDevice: failed to serializeDevice device...\r\n");
        return false;
    }


    char slot[5];
    snprintf(slot, 5, "%02i", s);

    const std::string path =
            p_fs->getDevicesDirectory() + "/" + device->getVendor() + "-" + device->getProduct()
            + "-" + slot + ".json";
    printf("Config::saveDevice: %s\r\n", path.c_str());

    const Fs::File file{path, Fs::File::OpenMode::Write};
    const int32_t wrote = file.write(0, buffer.size(), reinterpret_cast<const char *>(buffer.data()));

    return wrote == buffer.size();
}

bool Config::setSlot(const uint16_t vid, const uint16_t pid, const uint8_t slot) {
    for (auto p: m_profiles) {
        if (p.vid == vid && p.pid == pid) {
            p.slot = slot;
            return save();
        }
    }

    // profile not found, add it
    const Profile profile{.vid = vid, .pid = pid, .slot = slot};
    m_profiles.push_back(profile);
    return save();
}

uint8_t Config::getSlot(const uint16_t vid, const uint16_t pid) {
    for (const auto &p: m_profiles) {
        if (p.vid == vid && p.pid == pid) {
            return p.slot;
        }
    }

    return 0;
}
