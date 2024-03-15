//
// Created by cpasjuste on 13/03/24.
//

#ifndef U_GAMEPAD_HID_H
#define U_GAMEPAD_HID_H

#include <cstdint>
#include "hid_utility.h"

namespace uGamePad {
    class Hid {
    public:
        virtual void loop() {}

        void onDeviceConnected(Device *device);

        void onDeviceDisconnected(Device *device);

        void onDeviceInputReport(Device *device, uint8_t const *report, uint16_t len);
    };
}

#endif //U_GAMEPAD_HID_H
