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

#ifndef BIT
#define BIT(n) (1U<<(n))
#endif

#define INDEX_NONE UINT16_MAX
#define BUTTON_NONE UINT16_MAX

namespace uGamePad {
    class Report {
    public:
        enum AxisType : uint8_t {
            AXIS_NONE = BIT(0),
            AXIS_I16 = BIT(1),
            AXIS_UI8 = BIT(2),
            AXIS_FLIP_Y = BIT(3)
        };

        struct Button {
            uint16_t byte_index;
            uint16_t button_index;
        };

        struct Axis {
            uint16_t byte_index;
            uint8_t type;
        };

        struct Init {
            uint8_t msg[4];
            uint8_t size;
        };

        Button buttons[12]{};
        Axis axis[4]{};
        Button hat{};
        Init init{};
        uint8_t min_size = 4;
    };

    struct Device {
        uint16_t idVendor{};
        uint16_t idProduct{};
        char *name{};
        Report *report = nullptr;
    };
}

const uGamePad::Device *get_device(uint16_t vid, uint16_t pid);

const uGamePad::Device *get_device_at(uint16_t i);

#endif //U_GAMEPAD_DEVICES_H
