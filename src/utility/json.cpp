//
// Created by cpasjuste on 09/05/23.
//

#include "main.h"
#include "json.h"

using namespace uGamePad;

std::vector<uint8_t> Json::getDevice(Device *device) {
    if (!device) {
        printf("Json::getDocument: device is null...\r\n");
        return {};
    }

    // device info
    StaticJsonDocument<2048> doc;
    doc["name"] = device->name;
    doc["vendor"] = device->getVendor();
    doc["product"] = device->getProduct();
    // min data data size for a report to be valid
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

    std::vector<uint8_t> buffer(doc.capacity());
    size_t len = serializeJsonPretty(doc, buffer.data(), buffer.size());
    if (len == 0) {
        printf("Json::getDevice: failed to serialize device...\r\n");
        return {};
    }

    buffer.resize(len);
    return buffer;
}

Device *Json::getDevice(const std::vector<uint8_t> &buffer) {
    StaticJsonDocument<2048> doc;
    DeserializationError err = deserializeJson(doc, buffer.data(), buffer.size());
    if (err) {
        printf("Json::getDevice: failed to deserialize buffer: %s\r\n", err.c_str());
        return nullptr;
    }

    // create new device
    auto device = new Device();
    device->data = new ReportData();

    // device info
    strncpy(device->name, doc["name"], sizeof(device->name));
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

    return device;
}
