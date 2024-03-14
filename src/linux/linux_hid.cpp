//
// Created by cpasjuste on 11/03/24.
//

#include <cstdio>
#include <vector>
#include <algorithm>
#include "main.h"
#include "linux_hid.h"
#include "hidapi_libusb.h"
#include "hidparser.h"

using namespace uGamePad;

uint8_t report_buffer[HID_API_MAX_REPORT_DESCRIPTOR_SIZE];

// you need to create an udev rule with these devices for using libusb as a "normal user"
// /etc/udev/rules.d/50-ugamepad.rules
// SUBSYSTEM=="usb", ATTRS{idVendor}=="24c6", ATTRS{idProduct}=="550d", GROUP="plugdev", TAG+="uaccess"
// SUBSYSTEM=="usb", ATTRS{idVendor}=="0079", ATTRS{idProduct}=="0011", GROUP="plugdev", TAG+="uaccess"
std::vector<Hid::HidDevice> devices_white_list = {
        {0x24c6, 0x550d, ""},   // xbox 360 "ORI GEM"
        {0x0079, 0x0011, ""}    // cheap snes "USB Gamepad"
};

std::vector<Hid::HidDevice> devices_list;

LinuxHid::LinuxHid() {
    if (hid_init()) {
        printf("LinuxHid: hid_init failed\r\n");
        return;
    }

    m_inited = true;
}

void LinuxHid::loop() {
    // return if pad is not initialized yet (should not happen...)
    if (!getPlatform()->getPad()) return;

    // get device list
    struct hid_device_info *devices = hid_enumerate(0, 0);

    // add new devices
    auto cur_dev = devices;
    for (; cur_dev; cur_dev = cur_dev->next) {
        Hid::HidDevice hd = {cur_dev->vendor_id, cur_dev->product_id};
        if (cur_dev->product_string) wcstombs(hd.name, cur_dev->product_string, 63);

        // TODO: remove white list stuff
        // first check if the device is in white list
        auto it = std::find_if(devices_white_list.begin(), devices_white_list.end(), [hd](const HidDevice &device) {
            return hd.vid == device.vid && hd.pid == device.pid;
        });
        if (it == devices_white_list.end()) continue;

        // now check if it was already added
        it = std::find_if(devices_list.begin(), devices_list.end(), [hd](const HidDevice &device) {
            return hd.vid == device.vid && hd.pid == device.pid;
        });

        // skip if already added
        if (it != devices_list.end()) continue;

        // open device
        hd.data = hid_open_path(cur_dev->path);
        if (!hd.data) {
            printf("LinuxHid: could not open device %04x:%04x\r\n", hd.vid, hd.pid);
            continue;
        }

        // get report descriptor
        int size = hid_get_report_descriptor(
                (hid_device *) hd.data, report_buffer, HID_API_MAX_REPORT_DESCRIPTOR_SIZE);
        if (size < 32) {
            printf("LinuxHid: could not get report descriptor for %04x:%04x\r\n", hd.vid, hd.pid);
            continue;
        }

        // parse report descriptor
        if (!parse_report_descriptor(report_buffer, size, &hd.report)) {
            printf("LinuxHid: could not get report descriptor for %04x:%04x\r\n", hd.vid, hd.pid);
            continue;
        }

        // set non blocking polling
        hid_set_nonblocking((hid_device *) hd.data, 1);

        // add to devices list
        devices_list.push_back(hd);

        // notify
        onDeviceConnected(hd);
    }

    // remove disconnected devices
    for (int i = (int) (devices_list.size() - 1); i > -1; i--) {
        bool found = false;
        cur_dev = devices;
        for (; cur_dev; cur_dev = cur_dev->next) {
            if (devices_list[i].vid == cur_dev->vendor_id && devices_list[i].pid == cur_dev->product_id) {
                found = true;
                break;
            }
        }

        if (!found) {
            //printf("LinuxHid: device removed: %04x:%04x\r\n", devices_list[i].vid, devices_list[i].pid);
            onDeviceDisconnected(devices_list[i]);
            if (devices_list[i].data) hid_close((hid_device *) devices_list[i].data);
            devices_list.erase(devices_list.begin() + (int) i);
        }
    }

    // handle reports
    if (!devices_list.empty()) {
        cur_dev = devices;
        for (; cur_dev; cur_dev = cur_dev->next) {
            Hid::HidDevice hd = devices_list[0];
            if (hd.data && cur_dev->product_id == hd.pid && cur_dev->vendor_id == hd.vid) {
                report_buffer[0] = hd.report.report_id;
                int len = hid_read((hid_device *) hd.data, report_buffer, hd.report.report_size);
                if (len > 1) {
#if 0
                    printf("%s: ", hd.name);
                    for (int i = 0; i < len; i++) {
                        printf("0x%02x ", report_buffer[i]);
                        if ((i + 1) % 10 == 0) {
                            printf("\n");
                        }
                    }
                    printf("\r\n");
#endif
                    onDeviceInputReport(hd, report_buffer, len);
                }
            }
        }
    }

    hid_free_enumeration(devices);

    Hid::loop();
}

LinuxHid::~LinuxHid() {
    if (m_inited) {
        hid_exit();
    }
}
