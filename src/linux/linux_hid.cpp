//
// Created by cpasjuste on 11/03/24.
//

#include <cstdio>
#include <vector>
#include <algorithm>
#include "main.h"
#include "linux_hid.h"
#include "hidapi_libusb.h"
#include "hid_utility.h"

using namespace uGamePad;

// you need to create an udev rule with these devices for using libusb as a "normal user"
// "/etc/udev/rules.d/50-ugamepad.rules"
// SUBSYSTEM=="usb", ATTRS{idVendor}=="24c6", ATTRS{idProduct}=="550d", GROUP="plugdev", TAG+="uaccess"
// SUBSYSTEM=="usb", ATTRS{idVendor}=="0079", ATTRS{idProduct}=="0011", GROUP="plugdev", TAG+="uaccess"

uint8_t report_buffer[HID_API_MAX_REPORT_DESCRIPTOR_SIZE];

std::vector<Device *> devices_list;
std::vector<Device *> black_list;

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
        auto device = new Device{cur_dev->vendor_id, cur_dev->product_id};

        // check if device is in black list
        auto it = std::find_if(black_list.begin(), black_list.end(), [device](Device *d) {
            return device->vid == d->vid && device->pid == d->pid;
        });
        // skip if already added to black list
        if (it != black_list.end()) {
            delete (device);
            continue;
        }

        // now check if it was already added
        it = std::find_if(devices_list.begin(), devices_list.end(), [device](Device *d) {
            return device->vid == d->vid && device->pid == d->pid;
        });
        // skip if already added
        if (it != devices_list.end()) continue;

        // open device
        auto handle = hid_open_path(cur_dev->path);
        if (!handle) {
            printf("LinuxHid: could not open device %04x:%04x\r\n", device->vid, device->pid);
            black_list.push_back(device);
            continue;
        }

        // set non blocking polling
        hid_set_nonblocking(handle, 1);

        // set device name from device info
        if (cur_dev->product_string) wcstombs(device->name, cur_dev->product_string, 63);

        // check if device exists in user fs or device database
        auto userDevice = getPlatform()->getConfig()->loadDevice(device->vid, device->pid);
        if (userDevice) {
            delete (device);
            device = userDevice;
        } else {
            userDevice = get_device(device->vid, device->pid);
            if (userDevice) {
                delete (device);
                device = userDevice;
            }
        }

        // set device hid handle
        device->user_data = handle;

        // get and parse report descriptor if not set from "userDevice"
        if (!device->report) {
            device->report = (ReportData *) malloc(sizeof(ReportData));
            auto reportSize = hid_get_report_descriptor(handle, report_buffer, HID_API_MAX_REPORT_DESCRIPTOR_SIZE);
            if (!device->report || reportSize < 32) {
                printf("LinuxHid: could not get report descriptor for %04x:%04x\r\n", device->vid, device->pid);
                // free resources
                if (device->report) free(device->report);
                hid_close(handle);
                black_list.push_back(device);
                continue;
            }

            // parse report descriptor
            if (!parse_report_descriptor(report_buffer, reportSize, device)) {
                printf("LinuxHid: could not parse report descriptor for %04x:%04x\r\n", device->vid, device->pid);
                if (device->report) free(device->report);
                hid_close(handle);
                black_list.push_back(device);
                continue;
            }
        }

        // only handle joystick devices
        if (device->report->type != REPORT_TYPE_JOYSTICK) {
            printf("LinuxHid: device %04x:%04x is not a joystick, skipping...\r\n", device->vid, device->pid);
            if (device->report) free(device->report);
            hid_close(handle);
            black_list.push_back(device);
            continue;
        }

        // add to devices list
        devices_list.push_back(device);

        // notify
        onDeviceConnected(device);
    }

    // remove disconnected devices
    for (int i = (int) (devices_list.size() - 1); i > -1; i--) {
        bool found = false;
        cur_dev = devices;
        for (; cur_dev; cur_dev = cur_dev->next) {
            if (devices_list[i]->vid == cur_dev->vendor_id && devices_list[i]->pid == cur_dev->product_id) {
                found = true;
                break;
            }
        }

        if (!found) {
            printf("LinuxHid: device removed: %04x:%04x\r\n", devices_list[i]->vid, devices_list[i]->pid);
            auto device = devices_list[i];
            if (device->user_data) hid_close((hid_device *) device->user_data);
            devices_list.erase(devices_list.begin() + (int) i);
            onDeviceDisconnected(device);
            free(device->report);
            delete (device);
        }
    }

    // release hid devices
    hid_free_enumeration(devices);

    // handle reports (only one joystick supported...)
    if (!devices_list.empty()) {
        auto device = devices_list[0];
        if (device->user_data) {
            report_buffer[0] = device->report->report_id;
            int len = hid_read((hid_device *) device->user_data, report_buffer, device->report->report_size);
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
                onDeviceInputReport(device, report_buffer, len);
            }
        }
    }

    Hid::loop();
}

LinuxHid::~LinuxHid() {
    if (m_inited) {
        hid_exit();
    }
}
