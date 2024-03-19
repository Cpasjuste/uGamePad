//
// Created by cpasjuste on 25/03/23.
//

#ifndef LINUX

#include <pico/bootrom.h>
#include <hardware/watchdog.h>

#endif

#include <string>
#include "gamepad.h"
#include "utility.h"

#undef ARDUINO
#define ARDUINOJSON_ENABLE_STD_STREAM 1

#include "ArduinoJson-v7.0.3.h"

using namespace uGamePad;

void Utility::reboot(bool bootloader) {
#ifndef LINUX
    if (bootloader) {
#if PICO_BUILD
        reset_usb_boot(0, 0);
#else
        printf("Utility::reboot: reboot to bootloader not implemented...");
#endif
    }
    watchdog_reboot(0, 0, 50);
#endif
}

std::string Utility::toString(uint32_t buttons) {
    std::string ret;

    if (buttons & GamePad::Button::B1) ret += "B1 ";
    if (buttons & GamePad::Button::B2) ret += "B2 ";
    if (buttons & GamePad::Button::B3) ret += "B3 ";
    if (buttons & GamePad::Button::B4) ret += "B4 ";
    if (buttons & GamePad::Button::B5) ret += "B5 ";
    if (buttons & GamePad::Button::B6) ret += "B6 ";
    if (buttons & GamePad::Button::START) ret += "START ";
    if (buttons & GamePad::Button::SELECT) ret += "SELECT ";
    if (buttons & GamePad::Button::DPAD_UP) ret += "DPAD_UP ";
    if (buttons & GamePad::Button::DPAD_DOWN) ret += "DPAD_DOWN ";
    if (buttons & GamePad::Button::DPAD_LEFT) ret += "DPAD_LEFT ";
    if (buttons & GamePad::Button::DPAD_RIGHT) ret += "DPAD_RIGHT ";
    if (buttons & GamePad::Button::AXIS_UP) ret += "AXIS_UP ";
    if (buttons & GamePad::Button::AXIS_DOWN) ret += "AXIS_DOWN ";
    if (buttons & GamePad::Button::AXIS_LEFT) ret += "AXIS_LEFT ";
    if (buttons & GamePad::Button::AXIS_RIGHT) ret += "AXIS_RIGHT ";

    return ret;
}

std::string Utility::parseSize(uint64_t size) {
    char output[32];
    const char *suffix[] = {"B", "KB", "MB", "GB", "TB"};
    char length = sizeof(suffix) / sizeof(suffix[0]);
    auto dblBytes = (double) size;
    int i = 0;

    if (size <= 0) {
        return "0 B";
    } else {
        if (size > 1024) {
            for (i = 0; (size / 1024) > 0 && i < length - 1; i++, size /= 1024)
                dblBytes = (double) size / 1024.0;
        }

        snprintf(output, 31, "~%i %s", (int) dblBytes, suffix[i]);
        return output;
    }
}

std::string Utility::baseName(const std::string &path) {
    std::string name = path;
    if (path.size() > 1) {
        const size_t idx = path.find_last_of('/');
        if (idx != std::string::npos) {
            name.erase(0, idx + 1);
        }
    }
    return name;
}

bool Utility::serialize(Device *device, std::vector<uint8_t> *buffer) {
    if (!device) {
        printf("Json::getDevice: device is null...\r\n");
        return {};
    }

    // device info
    JsonDocument doc;
    doc["vid"] = device->getVendor();
    doc["pid"] = device->getProduct();
    doc["name"] = device->name;

    // input report
    JsonObject item = doc["input_descriptor"].to<JsonObject>();
    //item["type"] = device->report->type; // not needed for user
    //item["report_id"] = device->report->report_id;  // not needed for user
    item["report_size"] = device->report->report_size;
    item["is_xbox"] = device->report->is_xbox;

    // joystick
    item = doc["input_descriptor"]["joystick"].to<JsonObject>();
    item["button_count"] = device->report->joystick.button_count;

    // axis
    JsonArray array = doc["input_descriptor"]["joystick"]["axis"].to<JsonArray>();
    for (auto &axis: device->report->joystick.axis) {
        item = array.add<JsonObject>();
        item["offset"] = axis.offset;
        item["size"] = axis.size;
        item["logical"][0] = axis.logical.min;
        item["logical"][1] = axis.logical.max;
    }

    // buttons
    array = doc["input_descriptor"]["joystick"]["buttons"].to<JsonArray>();
    for (int i = 0; i < device->report->joystick.button_count; i++) {
        item = array.add<JsonObject>();
        item["byte_offset"] = device->report->joystick.buttons[i].byte_offset;
        item["bitmask"] = device->report->joystick.buttons[i].bitmask;
    }

    // hat
    item = doc["input_descriptor"]["joystick"]["hat"].to<JsonObject>();
    item["offset"] = device->report->joystick.hat.offset;
    item["size"] = device->report->joystick.hat.size;
    item["logical"][0] = device->report->joystick.hat.logical.min;
    item["logical"][1] = device->report->joystick.hat.logical.max;
    item["physical"][0] = device->report->joystick.hat.physical.min;
    item["physical"][1] = device->report->joystick.hat.physical.max;

    // init
    item = doc["input_descriptor"]["joystick"]["init"].to<JsonObject>();
    item["size"] = device->report->joystick.init.size;
    for (int i = 0; i < device->report->joystick.init.size; i++) {
        item["bytes"][i] = device->report->joystick.init.bytes[i];
    }

    size_t len = serializeJsonPretty(doc, buffer->data(), buffer->size());
    if (len == 0) {
        printf("Json::getDevice: failed to serialize device...\r\n");
        return {};
    }

    buffer->resize(len);
    return buffer;
}

Device *Utility::deserialize(const std::vector<uint8_t> *buffer) {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, buffer->data(), buffer->size());
    if (err) {
        printf("Json::getDevice: failed to deserialize buffer: %s\r\n", err.c_str());
        return nullptr;
    }

    // create new device
    auto device = new Device();
    device->report = new InputReportDescriptor();

    // device info
    sscanf(doc["vid"], "%4hx", &device->vid);
    sscanf(doc["pid"], "%4hx", &device->pid);
    strncpy(device->name, doc["name"], sizeof(device->name));

    //device->report->type = doc["input_descriptor"]["type"];  // not needed for user
    //device->report->report_id = doc["input_descriptor"]["report_id"];  // not needed for user
    device->report->type = REPORT_TYPE_JOYSTICK;
    device->report->report_size = doc["input_descriptor"]["report_size"];
    device->report->is_xbox = doc["input_descriptor"]["is_xbox"];

    // joystick
    device->report->joystick.button_count = doc["input_descriptor"]["joystick"]["button_count"];

    // axis
    JsonArray axis = doc["input_descriptor"]["joystick"]["axis"];
    for (int i = 0; i < MAX_AXIS; i++) {
        device->report->joystick.axis[i].offset = axis[i]["offset"];
        device->report->joystick.axis[i].size = axis[i]["size"];
        device->report->joystick.axis[i].logical.min = axis[i]["logical"][0];
        device->report->joystick.axis[i].logical.max = axis[i]["logical"][1];
    }

    // buttons
    JsonArray buttons = doc["input_descriptor"]["joystick"]["buttons"];
    for (int i = 0; i < device->report->joystick.button_count; i++) {
        device->report->joystick.buttons[i].byte_offset = buttons[i]["byte_offset"];
        device->report->joystick.buttons[i].bitmask = buttons[i]["bitmask"];
    }

    // hat
    device->report->joystick.hat.offset = doc["input_descriptor"]["joystick"]["hat"]["offset"];
    device->report->joystick.hat.size = doc["input_descriptor"]["joystick"]["hat"]["size"];
    device->report->joystick.hat.logical.min = doc["input_descriptor"]["joystick"]["hat"]["logical"][0];
    device->report->joystick.hat.logical.max = doc["input_descriptor"]["joystick"]["hat"]["logical"][1];
    device->report->joystick.hat.physical.min = doc["input_descriptor"]["joystick"]["hat"]["physical"][0];
    device->report->joystick.hat.physical.max = doc["input_descriptor"]["joystick"]["hat"]["physical"][1];

    // init
    device->report->joystick.init.size = doc["input_descriptor"]["joystick"]["init"]["size"];
    JsonArray bytes = doc["input_descriptor"]["joystick"]["init"]["bytes"];
    for (int i = 0; i < bytes.size(); i++) {
        device->report->joystick.init.bytes[i] = bytes[i];
    }

    return device;
}
