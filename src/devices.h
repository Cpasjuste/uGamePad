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

#define INPUT_DUMMY UINT8_MAX
#define MAX_BUTTONS 12
#define MAX_AXIS 4

#define AXIS_TYPE_U8        BIT(0)
#define AXIS_TYPE_S8        BIT(1)
#define AXIS_TYPE_S16       BIT(2)
#define AXIS_TYPE_FLIP_Y    BIT(3)

#define REPORT_TYPE_NONE        0
#define REPORT_TYPE_MOUSE       1
#define REPORT_TYPE_KEYBOARD    2
#define REPORT_TYPE_JOYSTICK    3

namespace uGamePad {
    typedef struct {
        uint8_t type: 2;
        uint8_t report_id;
        uint8_t report_size;
        bool is_xbox;

        union {
            struct {
                struct {
                    uint16_t offset;
                    uint8_t size;
                    struct {
                        int16_t min;
                        int16_t max;
                    } logical;
                } axis[MAX_AXIS];

                struct {
                    uint8_t byte_offset;
                    uint8_t bitmask;
                } buttons[MAX_BUTTONS];

                struct {
                    uint16_t offset;
                    uint8_t size;
                    struct {
                        uint16_t min;
                        uint16_t max;
                    } logical;
                    struct {
                        uint16_t min;
                        uint16_t max;
                    } physical;
                } hat;

                struct {
                    uint8_t bytes[32];
                    uint8_t size;
                } init;

                uint8_t button_count;

            } joystick;
        };
    } InputReportDescriptor;

    struct Device {
        uint16_t vid = 0;
        uint16_t pid = 0;
        char name[64]{};
        InputReportDescriptor *report = nullptr;
        void *user_data = nullptr;

        [[nodiscard]] std::string getVendor() const {
            char str[5];
            snprintf(str, 5, "%04x", vid);
            return str;
        }

        [[nodiscard]] std::string getProduct() const {
            char str[5];
            snprintf(str, 5, "%04x", pid);
            return str;
        }
    };
}

uGamePad::Device *get_device(uint16_t vid, uint16_t pid);

uGamePad::Device *get_device_at(uint16_t i);

#endif //U_GAMEPAD_DEVICES_H
