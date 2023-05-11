//
// Created by cpasjuste on 28/03/23.
//

#ifndef U_GAMEPAD_DEVICES_H
#define U_GAMEPAD_DEVICES_H

#include <cstdint>
#include <cstring>
#include <string>

#ifndef BIT
#define BIT(n) (1U<<(n))
#endif

#define INDEX_NONE UINT16_MAX
#define BUTTON_NONE UINT16_MAX

namespace uGamePad {
    class ReportData {
    public:
        enum AxisType : uint8_t {
            AXIS_NONE = BIT(0),
            AXIS_I16 = BIT(1),
            AXIS_UI8 = BIT(2),
            AXIS_FLIP_Y = BIT(3)
        };

        struct Button {
            uint16_t byte;
            uint16_t bit;
        };

        struct Axis {
            uint16_t byte;
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
        uint8_t min_report_size = 4;
    };

    struct Device {
        uint16_t vendor = 0;
        uint16_t product = 0;
        char name[64]{};
        ReportData *data = nullptr;

        [[nodiscard]] std::string getVendor() const {
            char str[5];
            snprintf(str, 5, "%04x", vendor);
            return str;
        }

        [[nodiscard]] std::string getProduct() const {
            char str[5];
            snprintf(str, 5, "%04x", product);
            return str;
        }
    };
}

const uGamePad::Device *get_device(uint16_t vid, uint16_t pid);

const uGamePad::Device *get_device_at(uint16_t i);

#endif //U_GAMEPAD_DEVICES_H
