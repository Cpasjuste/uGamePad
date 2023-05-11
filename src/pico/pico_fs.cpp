//
// Created by cpasjuste on 05/04/23.
//

#include <Adafruit_SPIFlash.h>
#include <msc/Adafruit_USBD_MSC.h>
#include <ArduinoJson.h>
#include "main.h"
#include "pico_fs.h"
#include "utility/json.h"

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
    if (!m_fatfs.exists(PicoFs::getDeviceDirectory().c_str())) {
        m_fatfs.mkdir(PicoFs::getDeviceDirectory().c_str());
    }

    m_available = true;
}

std::vector<uint8_t> PicoFs::readFile(const std::string &path) {
    if (!m_available) {
        printf("PicoFs::readFile: filesystem not available...\r\n");
        return {};
    }

    File32 file = m_fatfs.open(path.c_str(), FILE_READ);
    if (!file) {
        printf("PicoFs::readFile: could not open %s for reading...\r\n", path.c_str());
        return {};
    }

    std::vector<uint8_t> buffer(file.size());
    if (!file.readBytes(buffer.data(), buffer.size())) {
        printf("PicoFs::readFile: could not read %s...\r\n", path.c_str());
        file.close();
        return {};
    }

    file.close();
    return buffer;
}

bool PicoFs::writeFile(const std::string &path, const std::vector<uint8_t> &data) {
    if (!m_available) {
        printf("PicoFs::writeFile: filesystem not available...\r\n");
        return false;
    }

    File32 file = m_fatfs.open(path.c_str(), O_RDWR | O_CREAT | O_TRUNC);
    if (!file) {
        printf("PicoFs::writeFile: could not open %s for writing...\r\n", path.c_str());
        file.close();
        return false;
    }

    if (!file.write(data.data(), data.size())) {
        printf("PicoFs::writeFile: could not write to %s...\r\n", path.c_str());
        file.close();
        return false;
    }

    file.close();
    flash.syncBlocks();
    m_fatfs.cacheClear();
    return true;
}

Fs::DeviceInfo PicoFs::getDeviceInfo() {
    uint64_t size = m_fatfs.clusterCount() * m_fatfs.bytesPerCluster();
    auto used = (uint64_t) ((m_fatfs.clusterCount() - m_fatfs.freeClusterCount()) *
                            (uint64_t) m_fatfs.bytesPerCluster());
    return {size, used};
}

void PicoFs::setUsbMode(uGamePad::Fs::UsbMode mode) {
    if (!m_available) {
        printf("PicoFs::share: error: flash filesystem not available...\r\n");
        return;
    }

    if (mode == UsbMode::Msc) {
        usb_msc.setID("uGamePad", "FlashFs", "1.0");
        usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
        usb_msc.setCapacity(flash.size() / 512, 512);
        usb_msc.setUnitReady(true);
        if (!usb_msc.begin()) {
            printf("PicoFs::share: error: usb msc begin failed\r\n");
            return;
        }
    }

    Fs::setUsbMode(mode);
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
