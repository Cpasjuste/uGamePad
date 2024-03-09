//
// Created by cpasjuste on 28/03/23.
//

#include "main.h"
#include "devices.h"
#include "tusb.h"

#define MAX_REPORT  4

using namespace uGamePad;

// Each HID instance can have multiple reports
static struct {
    uint8_t report_count;
    tuh_hid_report_info_t report_info[MAX_REPORT];
} hid_info[CFG_TUH_HID];

static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len);

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    printf("mount_cb: vid: %x, pid: %x, addr: %i, instance: %i, len: %i\r\n",
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
    const Device *device = get_device(vid, pid);
#endif
    // a know controller was plugged in (see devices.c)
    if (device) {
        if (device->vendor != getPlatform()->getPad()->getDevice()->vendor ||
            device->product != getPlatform()->getPad()->getDevice()->product) {
            getPlatform()->getPad()->setDevice(device, dev_addr, instance);
            // send init message if provided
            if (device->data->init.size > 0) {
                tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT,
                                   &device->data->init.msg, device->data->init.size);
            }
        }
    } else {
        printf("mount_cb: unknown gamepad (%04x:%04x)\r\n", vid, pid);
    }

    // Parse data descriptor with built-in parser
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(
            hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("mount_cb: tuh_hid_receive_report failed\r\n");
    }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

    printf("tuh_hid_report_received_cb\r\n");

    switch (itf_protocol) {
        case HID_ITF_PROTOCOL_KEYBOARD:
            TU_LOG2("HID receive boot keyboard report\r\n");
            break;
        case HID_ITF_PROTOCOL_MOUSE:
            TU_LOG2("HID receive boot mouse report\r\n");
            break;
        default:
            // Generic report requires matching ReportID and contents with previous parsed report info
            process_generic_report(dev_addr, instance, report, len);
            break;
    }

    // continue to request to receive report
    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("Error: cannot request to receive report\r\n");
    }
}

//--------------------------------------------------------------------+
// Generic Report
//--------------------------------------------------------------------+
static void process_generic_report(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    (void) dev_addr;

    uint8_t const rpt_count = hid_info[instance].report_count;
    tuh_hid_report_info_t *rpt_info_arr = hid_info[instance].report_info;
    tuh_hid_report_info_t *rpt_info = nullptr;

    if (rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
        // Simple report without report ID as 1st byte
        rpt_info = &rpt_info_arr[0];
    } else {
        // Composite report, 1st byte is report ID, data starts from 2nd byte
        uint8_t const rpt_id = report[0];

        // Find report id in the array
        for (uint8_t i = 0; i < rpt_count; i++) {
            if (rpt_id == rpt_info_arr[i].report_id) {
                rpt_info = &rpt_info_arr[i];
                break;
            }
        }

        report++;
        len--;
    }

    if (!rpt_info) {
        printf("Couldn't find the report info for this report !\r\n");
        return;
    }

    // For complete list of Usage Page & Usage checkout src/class/hid/hid.h. For examples:
    // - Keyboard                     : Desktop, Keyboard
    // - Mouse                        : Desktop, Mouse
    // - Gamepad                      : Desktop, Gamepad
    // - Consumer Control (Media Key) : Consumer, Consumer Control
    // - System Control (Power key)   : Desktop, System Control
    // - Generic (vendor)             : 0xFFxx, xx
    if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP) {
        switch (rpt_info->usage) {
            case HID_USAGE_DESKTOP_KEYBOARD:
                TU_LOG1("HID receive keyboard report\r\n");
                break;

            case HID_USAGE_DESKTOP_MOUSE:
                TU_LOG1("HID receive mouse report\r\n");
                break;
            default:
                printf("HID receive unknown report\r\n");
                break;
        }
    }
}

extern "C" {
void tuh_mount_cb(uint8_t dev_addr) {
    // application set-up
    printf("A device with address %d is mounted\r\n", dev_addr);
    /*
    auto buffer = (uint8_t *) malloc(sizeof(tusb_desc_device_t));
    tuh_descriptor_get_device_sync(dev_addr, buffer, sizeof(tusb_desc_device_t));
    auto desc = (tusb_desc_device_t *) buffer;
    printf("desc: %04x:%04x\r\n", desc->idVendor, desc->iProduct);
    free(buffer);
    */
}

void tuh_umount_cb(uint8_t dev_addr) {
    // application tear-down
    printf("A device with address %d is unmounted \r\n", dev_addr);
}
}
