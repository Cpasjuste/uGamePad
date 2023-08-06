//
// Created by cpasjuste on 28/03/23.
//

#include <Arduino.h>
#include "tusb.h"

#include "main.h"
#include "devices.h"

using namespace uGamePad;

#define MAX_REPORT 4
uint8_t _report_count[CFG_TUH_HID];
tuh_hid_report_info_t _report_info_arr[CFG_TUH_HID][MAX_REPORT];

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, uint8_t const *report_desc, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    if (!getPlatform()->getPad()) return;

    //printf("mount_cb: vid: %x, pid: %x, addr: %i, instance: %i, len: %i\r\n",
    //       vid, pid, dev_addr, idx, desc_len);

    // try to find the gamepad device from devices table
    auto device = get_device(vid, pid);
    if (!device) {
        // gamepad not found from devices table, try from (user) flash filesystem
        device = getPlatform()->getConfig()->loadDevice(vid, pid);
    }

    // a know controller was plugged in (see devices.c)
    if (device) {
        if (device->vendor != getPlatform()->getPad()->getDevice()->vendor ||
            device->product != getPlatform()->getPad()->getDevice()->product) {
            getPlatform()->getPad()->setDevice(device, dev_addr, idx);
            // send init message if provided
            if (device->data->init.size > 0) {
                tuh_hid_set_report(dev_addr, idx, 5, HID_REPORT_TYPE_OUTPUT,
                                   &device->data->init.msg, device->data->init.size);
            }
        }
    } else {
        printf("mount_cb: unknown device %x:%x\r\n", vid, pid);
    }

    // Parse data descriptor with built-in parser
    _report_count[idx] = tuh_hid_parse_report_descriptor(
            _report_info_arr[idx], MAX_REPORT, report_desc, desc_len);

    if (!tuh_hid_receive_report(dev_addr, idx)) {
        printf("mount_cb: tuh_hid_receive_report failed\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("umount_cb: device unmounted (address: %d, instance: %d)\r\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    uint8_t const rpt_count = _report_count[instance];
    tuh_hid_report_info_t *rpt_info_arr = _report_info_arr[instance];
    tuh_hid_report_info_t *rpt_info = nullptr;
    uint16_t vid, pid;

    auto pad = (PicoGamePad *) getPlatform()->getPad();
    if (!pad) return;

    tuh_vid_pid_get(dev_addr, &vid, &pid);
    if (pad->getDevice()->product != pid || pad->getDevice()->vendor != vid) {
        printf("received_cb: skipping data, wrong vid or pid for %s...\r\n", pad->getDevice()->name);
        tuh_hid_receive_report(dev_addr, instance);
        return;
    }

    //printf("tuh_hid_report_received_cb: addr: %i, instance: %i, len: %i\r\n", dev_addr, instance, len);
    if (!pad->report(report, len)) {
        // try to handle generic pads
        if (rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
            // simple data without data ID as 1st byte
            rpt_info = &rpt_info_arr[0];
        } else {
            // composite data, 1st byte is data ID, data starts from 2nd byte
            uint8_t const rpt_id = report[0];
            // find data id in the arrray
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
            printf("couldn't find the data info for this data !\r\n");
            return;
        }

        //printf("usage %d, %d\n", rpt_info->usage_page, rpt_info->usage);
        if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP) {
            switch (rpt_info->usage) {
                case HID_USAGE_DESKTOP_MOUSE:
                case HID_USAGE_DESKTOP_KEYBOARD:
                    //TU_LOG1("HID receive mouse/keyboard data\n");
                    break;
                case HID_USAGE_DESKTOP_JOYSTICK: {
                    // TU_LOG1("HID receive joystick data\n");
                    struct JoyStickReport {
                        uint8_t axis[3];
                        uint8_t buttons;
                    };
                    auto *rep = reinterpret_cast<const JoyStickReport *>(report);
                    //printf("x %d y %d button %02x\r\n", rep->axis[0], rep->axis[1], rep->buttons);
                }
                    break;
                case HID_USAGE_DESKTOP_GAMEPAD: {
                    // TODO
                }
                    break;
                default:
                    break;
            }
        }
    }

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("received_cb: cannot request to receive data\r\n");
    }
}
