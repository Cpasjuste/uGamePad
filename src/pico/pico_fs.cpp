//
// Created by cpasjuste on 05/04/23.
//

#include <LittleFS.h>
#include <ArduinoJson.h>
#include "main.h"
#include "pico_fs.h"

using namespace uGamePad;

PicoFs::PicoFs() : Fs() {
    // init little-fs with auto-format
    LittleFSConfig cfg;
    cfg.setAutoFormat(true);
    LittleFS.setConfig(cfg);
    if (!LittleFS.begin()) {
        printf("PicoFs: filesystem init failed\r\n");
        return;
    }

    // be sure we still have enough space to work with
    auto info = PicoFs::getDeviceInfo();
    m_available = info.usedBytes < info.totalBytes;
}

Fs::DeviceInfo PicoFs::getDeviceInfo() {
    FSInfo64 info{};
    LittleFS.info64(info);
    return {info.totalBytes, info.usedBytes};
}

Device *PicoFs::load(uint16_t vid, uint16_t pid) {
    char vendor[5], product[5];
    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    std::string path = getHome() + vendor + "-" + product + ".cfg";
    return PicoFs::load(path);
}

Device *PicoFs::load(const std::string &path) {
    auto device = new Device();
    device->data = new ReportData();

    if (!m_available) {
        printf("PicoFs::load: filesystem not available...\r\n");
        return device;
    }

    File file = LittleFS.open(path.c_str(), "r");
    if (!file) {
        printf("PicoFs::load: could not open %s\r\n", path.c_str());
        return device;
    }

    // json doc deserialization
    StaticJsonDocument<1024> doc;
    DeserializationError err = deserializeJson(doc, file);
    if (err) {
        printf("PicoFs::load: failed to deserialize %s (err: %s)\r\n", path.c_str(), err.c_str());
        file.close();
        return device;
    }

    // device info
    strlcpy(device->name, doc["name"], sizeof(device->name));
    device->vendor = doc["vendor"];
    device->product = doc["product"];

    // buttons
    JsonArray buttons = doc["buttons"];
    for (int i = 0; i < MAX_BUTTONS; i++) {
        device->data->buttons[i].byte = buttons[i]["byte"];
        device->data->buttons[i].bit = buttons[i]["bit"];
    }

    // axis
    JsonArray axis = doc["axis"];
    for (int i = 0; i < MAX_AXIS; i++) {
        device->data->axis[i].byte = axis[i]["byte"];
        device->data->axis[i].type = axis[i]["type"];
    }

    // hat
    JsonArray hat = doc["hat"];
    if (hat) {
        device->data->hat.byte = hat[0]["byte"];
        device->data->hat.bit = hat[0]["bit"];
    }

    // TODO: init message

    file.close();

    return device;
}

bool PicoFs::save(Device *device) {
    if (!device || !m_available) {
        printf("PicoFs::save: filesystem not available...\r\n");
        return false;
    }

    char vendor[5], product[5];
    snprintf(vendor, 5, "%04x", device->vendor);
    snprintf(product, 5, "%04x", device->product);
    std::string path = getHome() + vendor + "-" + product + ".cfg";
    File file = LittleFS.open(path.c_str(), "w");
    if (!file) {
        printf("PicoFs::save: could not open %s for writing...\r\n", path.c_str());
        return false;
    }

    // json doc
    StaticJsonDocument<1024> doc;

    // device info
    doc["name"] = device->name;
    doc["vendor"] = vendor;
    doc["product"] = product;
    // min data data size
    doc["min_report_size"] = device->data->min_report_size;

    // buttons
    JsonArray b_array = doc.createNestedArray("buttons");
    for (auto &button: device->data->buttons) {
        JsonObject item = b_array.createNestedObject();
        item["byte"] = button.byte;
        item["bit"] = button.bit;
    }

    // axis
    JsonArray a_array = doc.createNestedArray("axis");
    for (auto &axis: device->data->axis) {
        JsonObject item = a_array.createNestedObject();
        item["byte"] = axis.byte;
        item["type"] = axis.type;
    }

    // hat
    if (device->data->hat.byte != INDEX_NONE) {
        JsonArray h_array = doc.createNestedArray("hat");
        JsonObject h_item = h_array.createNestedObject();
        h_item["byte"] = device->data->hat.byte;
        h_item["bit"] = device->data->hat.bit;
    }

    // TODO: init message

    if (serializeJsonPretty(doc, file) == 0) {
        printf("PicoFs::save: failed to write to %s...\r\n", path.c_str());
        file.close();
        return false;
    }

    file.close();
    printf("PicoFs::save: device configuration saved to %s\r\n", path.c_str());

#if TESTING
    printFile(path.c_str());
#endif

    return true;
}
