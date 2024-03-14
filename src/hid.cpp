//
// Created by cpasjuste on 13/03/24.
//

#include <cstdio>
#include "main.h"
#include "hid.h"

using namespace uGamePad;

void Hid::onDeviceConnected(const HidDevice &hidDevice) {
    printf("Hid::onDeviceConnected: %04x:%04x\r\n", hidDevice.vid, hidDevice.pid);

    // return if pad is not initialized yet (should not happen...)
    if (!getPlatform()->getPad()) return;

    // first try to find gamepad device configuration from (user) flash filesystem
    auto device = getPlatform()->getConfig()->loadDevice(hidDevice.vid, hidDevice.pid);

    // now try to find the gamepad device from devices table
    if (!device) {
        device = get_device(hidDevice.vid, hidDevice.pid);
    }

    // a know controller was plugged in (see devices.c)
    if (device) {
        // set device "name" (product)
        if (strlen(hidDevice.name)) strncpy(device->name, hidDevice.name, 63);
        // update pad device
        getPlatform()->getPad()->setDevice(device);
        // send init message if provided
        if (device->data->init.size > 0) {
            // TODO (x360 init seq)
            //tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT, &device->data->init.msg, device->data->init.size);
        }
    } else {
        printf("Hid::onDeviceConnected: unknown device (%04x:%04x)\r\n", hidDevice.vid, hidDevice.pid);
    }

    // Parse data descriptor with built-in parser
    // TODO
    /*
    hid_info[instance].report_count = tuh_hid_parse_report_descriptor(
            hid_info[instance].report_info, MAX_REPORT, desc_report, desc_len);

    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_mount_cb: tuh_hid_receive_report failed\r\n");
    }
    */

}

void Hid::onDeviceInputReport(const Hid::HidDevice &hidDevice, const uint8_t *report, uint16_t len) {
    auto pad = getPlatform()->getPad();
    if (!pad || pad->isUnknown()) return;

    if (pad->getDevice()->product != hidDevice.pid || pad->getDevice()->vendor != hidDevice.vid) {
        printf("Hid::onReportReceived: skipping data, wrong vid or pid for %s...\r\n", pad->getDevice()->name);
        // TODO
        //tuh_hid_receive_report(dev_addr, instance);
        return;
    }

#if 0
    printf("Hid::onReportReceived: %s (%04x:%04x), len: %i\r\n",
           hidDevice.name, hidDevice.vid, hidDevice.pid, len);
#endif
    pad->onHidReport(report, len);

    // TODO
    /*
    if (!tuh_hid_receive_report(dev_addr, instance)) {
        printf("tuh_hid_report_received_cb: cannot request to receive data\r\n");
    }
    */
}

void Hid::onDeviceDisconnected(const HidDevice &hidDevice) {
    printf("Hid::onDeviceDisconnected: %04x:%04x\r\n", hidDevice.vid, hidDevice.pid);
}
