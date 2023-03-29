//
// Created by cpasjuste on 28/03/23.
//

#include <Arduino.h>
#include "tusb.h"

#include "main.h"
#include "devices.h"
#include "gamepad.h"

//#define MAX_REPORT 4
//uint8_t _report_count[CFG_TUH_HID];
//tuh_hid_report_info_t _report_info_arr[CFG_TUH_HID][MAX_REPORT];

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t idx, uint8_t const *report_desc, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    //printf("tuh_hid_mount_cb: vid: %x, pid: %x, addr: %i, instance: %i, len: %i\r\n",
    //       vid, pid, dev_addr, idx, desc_len);

    auto device = find_device(vid, pid);
    if (device) {  // a know controller was plugged in (see devices.c)
        if (device->idVendor != gamePad.getDevice()->idVendor || device->idProduct != gamePad.getDevice()->idProduct) {
            gamePad.setCurrentDevice(device, dev_addr, idx);
            // set default led value
            if (device->type == TYPE_XBOX360) {
                uint8_t msg[3] = {0x01, 0x03, 0x02};
                tuh_hid_set_report(dev_addr, idx, 5, HID_REPORT_TYPE_OUTPUT, &msg, 3);
            }
        }
    } else {
        printf("tuh_hid_mount_cb: unknown device %x:%x\r\n", vid, pid);
    }

    // Parse report descriptor with built-in parser
    //_report_count[instance] = tuh_hid_parse_report_descriptor(
    //      _report_info_arr[instance], MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, idx)) {
        printf("tuh_hid_mount_cb: tuh_hid_receive_report failed\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("HID device address = %d, instance = %d is unmounted\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    //uint8_t const rpt_count = _report_count[instance];
    //tuh_hid_report_info_t *rpt_info_arr = _report_info_arr[instance];
    //tuh_hid_report_info_t *rpt_info = nullptr;
    uint16_t vid, pid;

    tuh_vid_pid_get(dev_addr, &vid, &pid);
    if (gamePad.getDevice()->idProduct != pid || gamePad.getDevice()->idVendor != vid) {
        printf("received_cb: skipping report, wrong vid or pid for %s...\r\n", gamePad.getDevice()->name);
        tuh_hid_receive_report(dev_addr, instance);
        return;
    }

    //printf("tuh_hid_report_received_cb: addr: %i, instance: %i, len: %i\r\n", dev_addr, instance, len);
    if (!gamePad.update(report, len)) {
        // TODO: handle this
#if 0
        if (rpt_count == 1 && rpt_info_arr[0].report_id == 0) {
            // Simple report without report ID as 1st byte
            rpt_info = &rpt_info_arr[0];
        } else {
            // Composite report, 1st byte is report ID, data starts from 2nd byte
            uint8_t const rpt_id = report[0];

            // Find report id in the arrray
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
            printf("Couldn't find the report info for this report !\n");
            return;
        }

        //printf("usage %d, %d\n", rpt_info->usage_page, rpt_info->usage);
        if (rpt_info->usage_page == HID_USAGE_PAGE_DESKTOP) {
            switch (rpt_info->usage) {
                case HID_USAGE_DESKTOP_GAMEPAD:
                case HID_USAGE_DESKTOP_MOUSE:
                case HID_USAGE_DESKTOP_KEYBOARD:
                    //TU_LOG1("HID receive mouse/keyboard report\n");
                    // Assume mouse follow boot report layout
                    //                process_mouse_report((hid_mouse_report_t const *)report);
                    break;
                case HID_USAGE_DESKTOP_JOYSTICK: {
                    // TU_LOG1("HID receive joystick report\n");
                    struct JoyStickReport {
                        uint8_t axis[3];
                        uint8_t buttons;
                        // 実際のところはしらん
                    };
                    auto *rep = reinterpret_cast<const JoyStickReport *>(report);
                    printf("x %d y %d button %02x\n", rep->axis[0], rep->axis[1], rep->buttons);
                    /*
                    auto &gp = uGamePad::getState();
                    gp.axis[0] = rep->axis[0];
                    gp.axis[1] = rep->axis[1];
                    gp.axis[2] = rep->axis[2];
                    gp.buttons = rep->buttons;
                    gp.convertButtonsFromAxis(0, 1);
                    */

                    // BUFFALO BGC-FC801
                    // VID = 0411, PID = 00c6
                }
                    break;
                default:
                    break;
            }
        }
#endif
    }

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_report_received_cb: cannot request to receive report\r\n");
    }
}
