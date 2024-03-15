//
// Created by cpasjuste on 13/03/24.
//

#include <cstdio>
#include "main.h"
#include "hid.h"

using namespace uGamePad;

void Hid::onDeviceConnected(Device *device) {
    printf("Hid::onDeviceConnected: %s (%04x:%04x)\r\n", device->name, device->vid, device->pid);

    // set gamepad device
    getPlatform()->getPad()->setDevice(device);

    // TODO (x360 init seq)
    /*
    // send init message if provided
    if (device->data->init.size > 0) {
        //tuh_hid_set_report(dev_addr, instance, 5, HID_REPORT_TYPE_OUTPUT, &device->data->init.msg, device->data->init.size);
    }
    */
}

void Hid::onDeviceInputReport(Device *device, const uint8_t *report, uint16_t len) {
    auto pad = getPlatform()->getPad();
    if (!pad->getDevice()) return;

    if (pad->getDevice()->pid != device->pid || pad->getDevice()->vid != device->vid) {
        printf("Hid::onReportReceived: skipping data, wrong vid or pid for %s...\r\n", pad->getDevice()->name);
        return;
    }

    pad->onHidReport(report, len);
}

void Hid::onDeviceDisconnected(Device *device) {
    printf("Hid::onDeviceDisconnected: %s (%04x:%04x)\r\n", device->name, device->vid, device->pid);
    delete (device->report);
    delete (device);
    getPlatform()->getPad()->setDevice(nullptr);
}
