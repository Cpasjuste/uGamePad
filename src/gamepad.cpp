//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include <cstdio>
#include "tusb.h"
#include "gamepad.h"
#include "game_controller_data.h"

#undef printf
#define printf Serial1.printf
#define MAX_REPORT 4

extern const PadData pads_data[];
PadData padData;

namespace {
    uint8_t _report_count[CFG_TUH_HID];
    tuh_hid_report_info_t _report_info_arr[CFG_TUH_HID][MAX_REPORT];

    const PadData *getPadData(uint16_t vid, uint16_t pid) {
        // lookup for a known game controller
        int i = 0;
        int p = INT32_MAX;

        while (p != 0) {
            p = pads_data[i].idProduct;
            if (p == pid && pads_data[i].idVendor == vid) {
                return &pads_data[i];
            }
            i++;
        }

        return nullptr;
    }

    bool isNintendo(uint16_t vid, uint16_t pid) {
        return vid == 0x057e && (pid == 0x2009 || pid == 0x2017);
    }

    struct DS4Report {
        // https://www.psdevwiki.com/ps4/DS4-USB

        struct Button1 {
            inline static constexpr int SQUARE = 1 << 4;
            inline static constexpr int CROSS = 1 << 5;
            inline static constexpr int CIRCLE = 1 << 6;
            inline static constexpr int TRIANGLE = 1 << 7;
        };

        struct Button2 {
            inline static constexpr int L1 = 1 << 0;
            inline static constexpr int R1 = 1 << 1;
            inline static constexpr int L2 = 1 << 2;
            inline static constexpr int R2 = 1 << 3;
            inline static constexpr int SHARE = 1 << 4;
            inline static constexpr int OPTIONS = 1 << 5;
            inline static constexpr int L3 = 1 << 6;
            inline static constexpr int R3 = 1 << 7;
        };

        uint8_t reportID;
        uint8_t stickL[2];
        uint8_t stickR[2];
        uint8_t buttons1;
        uint8_t buttons2;
        uint8_t ps: 1;
        uint8_t tpad: 1;
        uint8_t counter: 6;
        uint8_t triggerL;
        uint8_t triggerR;
        // ...

        [[nodiscard]] int getHat() const { return buttons1 & 15; }
    };

    struct DS5Report {
        uint8_t reportID;
        uint8_t stickL[2];
        uint8_t stickR[2];
        uint8_t triggerL;
        uint8_t triggerR;
        uint8_t counter;
        uint8_t buttons[3];
        // ...

        struct Button {
            inline static constexpr int SQUARE = 1 << 4;
            inline static constexpr int CROSS = 1 << 5;
            inline static constexpr int CIRCLE = 1 << 6;
            inline static constexpr int TRIANGLE = 1 << 7;
            inline static constexpr int L1 = 1 << 8;
            inline static constexpr int R1 = 1 << 9;
            inline static constexpr int L2 = 1 << 10;
            inline static constexpr int R2 = 1 << 11;
            inline static constexpr int SHARE = 1 << 12;
            inline static constexpr int OPTIONS = 1 << 13;
            inline static constexpr int L3 = 1 << 14;
            inline static constexpr int R3 = 1 << 15;
            inline static constexpr int PS = 1 << 16;
            inline static constexpr int TPAD = 1 << 17;
        };

        [[nodiscard]] int getHat() const { return buttons[0] & 15; }
    };

    struct XBOXReport {
        // https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/
        uint8_t type;
        uint8_t len;
        uint16_t buttons;
        uint8_t triggers[2];
        uint16_t stickL[2];
        uint16_t stickR[2];
        uint8_t unused[6];

        struct Button {
            inline static constexpr int R3 = 1 << 0;
            inline static constexpr int L3 = 1 << 1;
            inline static constexpr int BACK = 1 << 2;
            inline static constexpr int START = 1 << 3;
            inline static constexpr int PAD_RIGHT = 1 << 4;
            inline static constexpr int PAD_LEFT = 1 << 5;
            inline static constexpr int PAD_DOWN = 1 << 6;
            inline static constexpr int PAD_UP = 1 << 7;
            inline static constexpr int LB = 1 << 8;
            inline static constexpr int RB = 1 << 9;
            inline static constexpr int GUIDE = 1 << 10;
            inline static constexpr int A = 1 << 12;
            inline static constexpr int B = 1 << 13;
            inline static constexpr int X = 1 << 14;
            inline static constexpr int Y = 1 << 15;
        };
    };
}

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);

    //printf("mount_cb: %x:%x\r\n", vid, pid);

    auto data = getPadData(vid, pid);
    if (data) {
        if (data->idVendor != padData.idVendor || data->idProduct != padData.idProduct) {
            printf("new gamepad discovered: %s\r\n", data->name);
            memcpy(&padData, data, sizeof(PadData));
        }
        if (padData.type == TYPE_XBOX360) {
            // disable led
            uint8_t msg[3] = {0x01, 0x03, 0x02};
            tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT, &msg, 3);
        }
    }

    // Parse report descriptor with built-in parser
    _report_count[instance] = tuh_hid_parse_report_descriptor(
            _report_info_arr[instance], MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("Error: cannot request to receive report\r\n");
    }
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
    printf("HID device address = %d, instance = %d is unmounted\n", dev_addr, instance);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    uint8_t const rpt_count = _report_count[instance];
    tuh_hid_report_info_t *rpt_info_arr = _report_info_arr[instance];
    tuh_hid_report_info_t *rpt_info = nullptr;

    uint16_t vid, pid;
    tuh_vid_pid_get(dev_addr, &vid, &pid);
    if (padData.idProduct != pid || padData.idVendor != vid) {
        printf("received_cb: skipping report, wrong vid or pid for %s...\r\n", padData.name);
        tuh_hid_receive_report(dev_addr, instance);
        return;
    }

    if (padData.type == TYPE_XBOX360) {
        //printf("tuh_hid_report_received_cb: %s (xbox)\r\n", data->name);
        if (sizeof(XBOXReport) == len && report[0] == 0x00) {
            auto r = reinterpret_cast<const XBOXReport *>(report);
            auto &gp = io::getCurrentGamePadState(0);
            gp.axis[0] = r->stickL[0];
            gp.axis[1] = r->stickL[1];
            gp.buttons =
                    (r->buttons & XBOXReport::Button::B ? io::GamePadState::Button::B : 0) |
                    (r->buttons & XBOXReport::Button::A ? io::GamePadState::Button::A : 0) |
                    (r->buttons & XBOXReport::Button::X ? io::GamePadState::Button::X : 0) |
                    (r->buttons & XBOXReport::Button::Y ? io::GamePadState::Button::Y : 0) |
                    (r->buttons & XBOXReport::Button::BACK ? io::GamePadState::Button::SELECT : 0) |
                    (r->buttons & XBOXReport::Button::START ? io::GamePadState::Button::START : 0);
            //gp.hat = static_cast<io::GamePadState::Hat>(r->getHat());
            //gp.convertButtonsFromAxis(0, 1);
            //gp.convertButtonsFromHat();
            if (gp.buttons != 0) printf("xbox: %lu\r\n", gp.buttons);
        } else {
            //printf("tuh_hid_report_received_cb: skipping report, wrong packet (xbox)\r\n");
            tuh_hid_receive_report(dev_addr, instance);
            return;
        }
    } else if (padData.type == TYPE_DS4) {
        //printf("tuh_hid_report_received_cb: len: %i\r\n", len);
        if (sizeof(DS4Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS4Report *>(report);
            auto &gp = io::getCurrentGamePadState(0);
            gp.axis[0] = r->stickL[0];
            gp.axis[1] = r->stickL[1];
            gp.buttons =
                    (r->buttons1 & DS4Report::Button1::CROSS ? io::GamePadState::Button::B : 0) |
                    (r->buttons1 & DS4Report::Button1::CIRCLE ? io::GamePadState::Button::A : 0) |
                    (r->buttons1 & DS4Report::Button1::TRIANGLE ? io::GamePadState::Button::X : 0) |
                    (r->buttons1 & DS4Report::Button1::SQUARE ? io::GamePadState::Button::Y : 0) |
                    (r->buttons2 & DS4Report::Button2::SHARE ? io::GamePadState::Button::SELECT : 0) |
                    (r->tpad ? io::GamePadState::Button::SELECT : 0) |
                    (r->buttons2 & DS4Report::Button2::OPTIONS ? io::GamePadState::Button::START : 0);
            gp.hat = static_cast<io::GamePadState::Hat>(r->getHat());
            gp.convertButtonsFromAxis(0, 1);
            gp.convertButtonsFromHat();
            if (gp.buttons != 0) printf("ds4: %lu\r\n", gp.buttons);
        } else {
            //printf("tuh_hid_report_received_cb: skipping report, wrong packet (ds4)\r\n");
            tuh_hid_receive_report(dev_addr, instance);
            return;
        }
    } else if (padData.type == TYPE_DS5) {
        if (sizeof(DS5Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS5Report *>(report);
            auto buttons = r->buttons[0] | (r->buttons[1] << 8) | (r->buttons[2] << 16);
            auto &gp = io::getCurrentGamePadState(0);
            gp.axis[0] = r->stickL[0];
            gp.axis[1] = r->stickL[1];
            gp.buttons =
                    (buttons & DS5Report::Button::CROSS ? io::GamePadState::Button::B : 0) |
                    (buttons & DS5Report::Button::CIRCLE ? io::GamePadState::Button::A : 0) |
                    (buttons & DS5Report::Button::TRIANGLE ? io::GamePadState::Button::X : 0) |
                    (buttons & DS5Report::Button::SQUARE ? io::GamePadState::Button::Y : 0) |
                    (buttons & (DS5Report::Button::SHARE | DS5Report::Button::TPAD) ? io::GamePadState::Button::SELECT
                                                                                    : 0) |
                    (buttons & DS5Report::Button::OPTIONS ? io::GamePadState::Button::START : 0);
            gp.hat = static_cast<io::GamePadState::Hat>(r->getHat());
            gp.convertButtonsFromAxis(0, 1);
            gp.convertButtonsFromHat();
        } else {
            printf("Invalid DS5 report size %zd\n", len);
            return;
        }
    } else if (isNintendo(vid, pid)) {
        printf("Nintendo: len = %d\n", len);
    } else {
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

        //        printf("usage %d, %d\n", rpt_info->usage_page, rpt_info->usage);

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
                    auto &gp = io::getCurrentGamePadState(0);
                    gp.axis[0] = rep->axis[0];
                    gp.axis[1] = rep->axis[1];
                    gp.axis[2] = rep->axis[2];
                    gp.buttons = rep->buttons;
                    gp.convertButtonsFromAxis(0, 1);

                    // BUFFALO BGC-FC801
                    // VID = 0411, PID = 00c6
                }
                    break;
                default:
                    break;
            }
        }
    }

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("Error: cannot request to receive report\r\n");
    }
}
