//
// Created by cpasjuste on 28/03/23.
//

#ifndef U_GAMEPAD_DEVICES_H
#define U_GAMEPAD_DEVICES_H

#include <cstdint>

#ifdef NATIVE
#define PROGMEM
#else
#include <avr/pgmspace.h>
#endif

#define TYPE_UNKNOWN        0
#define TYPE_XBOX           1
#define TYPE_XBOX360        2
#define TYPE_XBOX360W       3
#define TYPE_XBOX_ONE       4
#define TYPE_DS4            5
#define TYPE_DS5            6
#define TYPE_NEOGEO_MINI    7

#define INDEX_NONE UINT16_MAX
#define BUTTON_NONE UINT16_MAX

namespace uGamePad {
    class Report {
    public:
        enum AxisType : uint8_t {
            AXIS_32767 = 0,
            AXIS_255 = 4,
            AXIS_FLIP_Y = 8
        };
        struct ReportButton {
            uint16_t byte_index;
            uint16_t button_index;
        };

        struct ReportAxis {
            uint16_t byte_index;
            AxisType type;
        };

        ReportButton buttons[12]{};
        ReportAxis axis[4]{};
        ReportButton hat{};
        uint8_t min_bytes = 4;
    };

    struct Device {
        uint16_t idVendor{};
        uint16_t idProduct{};
        char *name{};
        uint8_t type{};
        Report *report = nullptr;
    };
}

const uGamePad::Device *find_device(uint16_t vid, uint16_t pid);

#endif //U_GAMEPAD_DEVICES_H
