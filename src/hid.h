//
// Created by cpasjuste on 13/03/24.
//

#ifndef U_GAMEPAD_HID_H
#define U_GAMEPAD_HID_H

#include <cstdint>
#include "hidparser.h"

namespace uGamePad {
    class Hid {
    public:
        struct HidDevice {
            uint16_t vid = 0;
            uint16_t pid = 0;
            char name[64]{};
            void *data = nullptr;
            hid_report_t report{};
        };

        virtual void loop() {}

        void onDeviceConnected(const HidDevice &hidDevice);

        void onDeviceDisconnected(const HidDevice &hidDevice);

        void onDeviceInputReport(const HidDevice &hidDevice, uint8_t const *report, uint16_t len);
    };
}

#endif //U_GAMEPAD_HID_H
