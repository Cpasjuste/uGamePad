#ifndef U_GAMEPAD_HID_PARSER_H
#define U_GAMEPAD_HID_PARSER_H

#define REPORT_TYPE_NONE     0
#define REPORT_TYPE_MOUSE    1
#define REPORT_TYPE_KEYBOARD 2
#define REPORT_TYPE_JOYSTICK 3

#define MAX_AXES 4

typedef struct {
    uint8_t type: 2;
    uint8_t report_id;
    uint8_t report_size;

    union {
        struct {
            struct {
                uint16_t offset;
                uint8_t size;
                struct {
                    uint16_t min;
                    uint16_t max;
                } logical;
            } axis[MAX_AXES];

            struct {
                uint8_t byte_offset;
                uint8_t bitmask;
            } button[12];

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

            uint8_t button_count;

        } joystick;
    };
} hid_report_t;

bool parse_report_descriptor(uint8_t *rep, uint16_t rep_size, hid_report_t *conf);

#endif // U_GAMEPAD_HID_PARSER_H
