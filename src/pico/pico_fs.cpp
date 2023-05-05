//
// Created by cpasjuste on 05/04/23.
//

#include <Adafruit_SPIFlash.h>
#include <msc/Adafruit_USBD_MSC.h>
#include <ArduinoJson.h>
#include "main.h"
#include "pico_fs.h"

#include "fat12/ff_format.h"
Adafruit_USBD_MSC usb_msc;
extern Adafruit_SPIFlash flash;
FatVolume m_fatfs;

int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize);

int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize);

void msc_flush_cb();

using namespace uGamePad;

PicoFs::PicoFs() : Fs() {
    if (!flash.begin()) {
        printf("PicoFs: failed to initialize flash chip!\r\n");
        return;
    }

    if (!m_fatfs.begin(&flash)) {
        printf("PicoFs: failed to mount fat partition, formatting...\r\n");
        format_fat12();
        if (!m_fatfs.begin(&flash)) {
            printf("PicoFs: failed to mount fat partition, formatting failed...\r\n");
            return;
        }
    }

    // create devices directory if needed
    if (!m_fatfs.exists(PicoFs::getDevicesDirectory().c_str())) {
        m_fatfs.mkdir(PicoFs::getDevicesDirectory().c_str());
    }

    m_available = true;
}

Fs::DeviceInfo PicoFs::getDeviceInfo() {
    // TODO: move to fatfs
    return {flash.size(), 0};
}

Device *PicoFs::load(uint16_t vid, uint16_t pid) {
    char vendor[5], product[5];
    snprintf(vendor, 5, "%04x", vid);
    snprintf(product, 5, "%04x", pid);
    std::string path = getDevicesDirectory() + "/" + vendor + "-" + product + ".cfg";
    return PicoFs::load(path);
}

Device *PicoFs::load(const std::string &path) {
    if (!m_available) {
        printf("PicoFs::load: filesystem not available...\r\n");
        return nullptr;
    }

    File32 file = m_fatfs.open(path.c_str(), FILE_READ);
    if (!file) {
        printf("PicoFs::load: failed to open %s\r\n", path.c_str());
        return nullptr;
    }

    // json doc deserialization
    StaticJsonDocument<1024> doc;
    DeserializationError err = deserializeJson(doc, file);
    if (err) {
        printf("PicoFs::load: failed to deserialize %s (err: %s)\r\n", path.c_str(), err.c_str());
        file.close();
        return nullptr;
    }

    // create new device
    auto device = new Device();
    device->data = new ReportData();

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
    std::string path = getDevicesDirectory() + "/" + vendor + "-" + product + ".cfg";

    File32 file = m_fatfs.open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC);
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
    flash.syncBlocks();
    m_fatfs.cacheClear();
    printf("PicoFs::save: device configuration saved to %s\r\n", path.c_str());

    return true;
}

void PicoFs::share() {
    if (!m_available) {
        printf("PicoFs::share: error: flash filesystem not available...\r\n");
        return;
    }

    usb_msc.setID("uGamePad", "FlashFs", "1.0");
    usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
    usb_msc.setCapacity(flash.size() / 512, 512);
    usb_msc.setUnitReady(true);
    if (!usb_msc.begin()) {
        printf("PicoFs::share: error: usb msc begin failed\r\n");
        return;
    }

    m_shared = true;
}

/*
 * USB MSC CALLBACKS
 */

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and
// return number of copied bytes (must be multiple of block size)
int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize) {
    // Note: SPIFLash Block API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
    return flash.readBlocks(lba, (uint8_t *) buffer, bufsize / 512) ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize) {
    // Note: SPIFLash Block API: readBlocks/writeBlocks/syncBlocks
    // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
    return flash.writeBlocks(lba, buffer, bufsize / 512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb() {
    // sync with flash
    flash.syncBlocks();
    // clear file system's cache to force refresh
    m_fatfs.cacheClear();
}
