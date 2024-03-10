//
// Created by cpasjuste on 10/03/24.
//

#include <cstring>
#include <bsp/board_api.h>
#include <device/usbd.h>
#include <class/msc/msc_device.h>
#include "pico_fs.h"
#include "pico_flash.h"

// https://github.com/32blit/32blit-sdk/blob/dba448a1c6e776d704fb299647786a30cef20478/32blit-pico/usb_device.cpp

using namespace uGamePad;

static bool storage_ejected = false;

void tud_mount_cb() {
    storage_ejected = false;
}

void tud_msc_inquiry_cb(uint8_t lun, uint8_t vendor_id[8], uint8_t product_id[16], uint8_t product_rev[4]) {
    (void) lun;

    const char vid[] = "uGamePad";
    const char pid[] = "uGamePad FlashFs";
    const char rev[] = "1.0";

    memcpy(vendor_id, vid, strlen(vid));
    memcpy(product_id, pid, strlen(pid));
    memcpy(product_rev, rev, strlen(rev));
}

bool tud_msc_test_unit_ready_cb(uint8_t lun) {
    if (storage_ejected) {
        tud_msc_set_sense(lun, SCSI_SENSE_NOT_READY, 0x3a, 0x00);
        return false;
    }

    return true;
}

void tud_msc_capacity_cb(uint8_t lun, uint32_t *block_count, uint16_t *block_size) {
    (void) lun;

    io_flash_get_size(*block_size, *block_count);
}

bool tud_msc_start_stop_cb(uint8_t lun, uint8_t power_condition, bool start, bool load_eject) {
    (void) lun;
    (void) power_condition;

    if (load_eject) {
        if (start) {
        } else
            storage_ejected = true;
    }

    return true;
}

int32_t tud_msc_read10_cb(uint8_t lun, uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize) {
    (void) lun;

    return io_flash_read(lba, offset, buffer, bufsize);
}

int32_t tud_msc_write10_cb(uint8_t lun, uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize) {
    (void) lun;

    return io_flash_write(lba, offset, buffer, bufsize);
}

int32_t tud_msc_scsi_cb(uint8_t lun, uint8_t const scsi_cmd[16], void *buffer, uint16_t bufsize) {
    uint16_t resplen = 0;

    switch (scsi_cmd[0]) {
        case SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL:
            // Host is about to read/write etc ... better not to disconnect disk
            resplen = 0;
            break;

        default:
            // Set Sense = Invalid Command Operation
            tud_msc_set_sense(lun, SCSI_SENSE_ILLEGAL_REQUEST, 0x20, 0x00);

            // negative means error -> tinyusb could stall and/or response with failed status
            resplen = -1;
            break;
    }

    return resplen;
}

bool tud_msc_is_writable_cb(uint8_t lun) {
    //return !get_files_open();
    return true;
}

void uGamePad::PicoFs::setUsbMode(uGamePad::Fs::UsbMode mode) {
    if (!m_available) {
        printf("PicoFs::share: error: flash filesystem not available...\r\n");
        return;
    }

    if (mode == UsbMode::Msc) {
        board_init();

        // init usb stack
        if (!tusb_init()) {
            printf("PicoFs::setUsbMode: tusb_init failed...\r\n");
            while (true);
        }

        // init device stack on configured roothub port
        if (!tud_init(BOARD_TUD_RHPORT)) {
            printf("PicoFs::setUsbMode: tud_init failed...\r\n");
            while (true);
        }
    } else {
        // init board
        board_init();

        // init usb stack
        if (!tusb_init()) {
            printf("PicoFs::setUsbMode: tusb_init failed...\r\n");
            while (true);
        }

        // init usb host stack
        if (!tuh_init(BOARD_TUH_RHPORT)) {
            printf("PicoFs::setUsbMode: tuh_init failed...\r\n");
            while (true);
        }
    }

    Fs::setUsbMode(mode);
}
