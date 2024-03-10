//
// Created by cpasjuste on 28/03/23.
//

#include "main.h"
#include "devices.h"
#include "tusb.h"

using namespace uGamePad;

#define MAX_REPORT  4
#define LANGUAGE_ID 0x0409 // english

static struct {
    uint8_t report_count;
    tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

static int count_utf8_bytes(const uint16_t *buf, size_t len);

static void convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len);

static char *convert_utf8_to_char(uint16_t *temp_buf, size_t buf_len);

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    printf("tuh_hid_mount_cb: vid: %x, pid: %x, addr: %i, instance: %i, len: %i\r\n",
           vid, pid, dev_addr, instance, desc_len);

    // return if pad is not initialized yet (should not happen...)
    if (!getPlatform()->getPad()) return;

    // first try to find gamepad device configuration from (user) flash filesystem
#warning "TODO: port config"
#if 0
    const Device *device = getPlatform()->getConfig()->loadDevice(vid, pid);
    if (!device) {
        // now try to find the gamepad device from devices table
        device = get_device(vid, pid);
    }
#else
    auto device = get_device(vid, pid);
#endif
    // a know controller was plugged in (see devices.c)
    if (device) {
        // get device "name" (product)
        uint16_t buf[128];
        if (tuh_descriptor_get_product_string_sync(
                dev_addr, LANGUAGE_ID, buf, sizeof(buf)) == XFER_RESULT_SUCCESS) {
            auto p = convert_utf8_to_char(buf, TU_ARRAY_SIZE(buf));
            strncpy(device->name, p, 63);
        }

        getPlatform()->getPad()->setDevice(device, dev_addr, instance);
        // send init message if provided
        if (device->data->init.size > 0) {
            tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT,
                               &device->data->init.msg, device->data->init.size);
        }
    } else {
        printf("tuh_hid_mount_cb: unknown gamepad (%04x:%04x)\r\n", vid, pid);
    }

    // Parse data descriptor with built-in parser
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(
            hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_mount_cb: tuh_hid_receive_report failed\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("tuh_hid_umount_cb: device unmounted (address: %d, instance: %d)\r\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    uint16_t vid, pid;
#if 0
    uint8_t const rpt_count = hid_info[instance].report_count;
    tuh_hid_report_info_t *rpt_info_arr = hid_info[instance].report_info;
    tuh_hid_report_info_t *rpt_info = nullptr;
#endif

    auto pad = (PicoGamePad *) getPlatform()->getPad();
    if (!pad || pad->isUnknown()) return;

    tuh_vid_pid_get(dev_addr, &vid, &pid);
    if (pad->getDevice()->product != pid || pad->getDevice()->vendor != vid) {
        printf("tuh_hid_report_received_cb: skipping data, wrong vid or pid for %s...\r\n", pad->getDevice()->name);
        tuh_hid_receive_report(dev_addr, instance);
        return;
    }

    //printf("tuh_hid_report_received_cb: addr: %i, instance: %i, len: %i\r\n", dev_addr, instance, len);
    pad->report(report, len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_report_received_cb: cannot request to receive data\r\n");
    }
}

static int count_utf8_bytes(const uint16_t *buf, size_t len) {
    size_t total_bytes = 0;
    for (size_t i = 0; i < len; i++) {
        uint16_t chr = buf[i];
        if (chr < 0x80) {
            total_bytes += 1;
        } else if (chr < 0x800) {
            total_bytes += 2;
        } else {
            total_bytes += 3;
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
    return (int) total_bytes;
}

static void convert_utf16le_to_utf8(const uint16_t *utf16, size_t utf16_len, uint8_t *utf8, size_t utf8_len) {
    // TODO: Check for runover.
    (void) utf8_len;
    // Get the UTF-16 length out of the data itself.

    for (size_t i = 0; i < utf16_len; i++) {
        uint16_t chr = utf16[i];
        if (chr < 0x80) {
            *utf8++ = chr & 0xffu;
        } else if (chr < 0x800) {
            *utf8++ = (uint8_t) (0xC0 | (chr >> 6 & 0x1F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 0 & 0x3F));
        } else {
            // TODO: Verify surrogate.
            *utf8++ = (uint8_t) (0xE0 | (chr >> 12 & 0x0F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 6 & 0x3F));
            *utf8++ = (uint8_t) (0x80 | (chr >> 0 & 0x3F));
        }
        // TODO: Handle UTF-16 code points that take two entries.
    }
}

static char *convert_utf8_to_char(uint16_t *temp_buf, size_t buf_len) {
    if ((temp_buf[0] & 0xff) == 0) return nullptr;  // empty
    size_t utf16_len = ((temp_buf[0] & 0xff) - 2) / sizeof(uint16_t);
    size_t utf8_len = (size_t) count_utf8_bytes(temp_buf + 1, utf16_len);
    convert_utf16le_to_utf8(temp_buf + 1, utf16_len, (uint8_t *) temp_buf, sizeof(uint16_t) * buf_len);
    ((uint8_t *) temp_buf)[utf8_len] = '\0';
    return (char *) temp_buf;
}

extern "C" {
void tuh_mount_cb(uint8_t dev_addr) {
    printf("tuh_mount_cb: new device connected, address: %d\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr) {
    printf("tuh_umount_cb: device disconnected, address: %d\r\n", dev_addr);
}
}
