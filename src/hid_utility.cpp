// http://www.frank-zhao.com/cache/hid_tutorial_1.php
// https://github.com/mist-devel/mist-firmware/blob/master/usb/hid.c

#include <cstring>
#include "hid_utility.h"

typedef struct {
    uint8_t bSize: 2;
    uint8_t bType: 2;
    uint8_t bTag: 4;
} __attribute__((packed)) item_t;

#ifdef NDEBUG
#define printf(fmt, ...) (0)
#endif
#define printf(fmt, ...) (0)

// flags for joystick components required
#define JOY_MOUSE_REQ_AXIS_X  0x01
#define JOY_MOUSE_REQ_AXIS_Y  0x02
#define JOY_MOUSE_REQ_BTN_0   0x04
#define JOY_MOUSE_REQ_BTN_1   0x08
#define JOYSTICK_COMPLETE     (JOY_MOUSE_REQ_AXIS_X | JOY_MOUSE_REQ_AXIS_Y | JOY_MOUSE_REQ_BTN_0)
#define MOUSE_COMPLETE        (JOY_MOUSE_REQ_AXIS_X | JOY_MOUSE_REQ_AXIS_Y | JOY_MOUSE_REQ_BTN_0 | JOY_MOUSE_REQ_BTN_1)

#define USAGE_PAGE_GENERIC_DESKTOP  1
#define USAGE_PAGE_SIMULATION       2
#define USAGE_PAGE_VR               3
#define USAGE_PAGE_SPORT            4
#define USAGE_PAGE_GAMING           5
#define USAGE_PAGE_GENERIC_DEVICE   6
#define USAGE_PAGE_KEYBOARD         7
#define USAGE_PAGE_LEDS             8
#define USAGE_PAGE_BUTTON           9
#define USAGE_PAGE_ORDINAL         10
#define USAGE_PAGE_TELEPHONY       11
#define USAGE_PAGE_CONSUMER        12


#define USAGE_POINTER   1
#define USAGE_MOUSE     2
#define USAGE_JOYSTICK  4
#define USAGE_GAMEPAD   5
#define USAGE_KEYBOARD  6
#define USAGE_KEYPAD    7
#define USAGE_MULTIAXIS 8

#define USAGE_X       48
#define USAGE_Y       49
#define USAGE_Z       50
#define USAGE_RX      51
#define USAGE_RY      52
#define USAGE_RZ      53
#define USAGE_WHEEL   56
#define USAGE_HAT     57

using namespace uGamePad;

// collect bits from byte stream and assemble them into a signed word
int16_t parse_joystick_bits(const uint8_t *p, uint16_t offset, uint8_t size, bool is_signed) {
    // mask unused bits of first byte
    uint8_t mask = 0xff << (offset & 7);
    uint8_t byte = offset / 8;
    uint8_t bits = size;
    uint8_t shift = offset & 7;

    uint16_t r_val = (p[byte++] & mask) >> shift;
    mask = 0xff;
    shift = 8 - shift;
    bits -= shift;

    // first byte already contained more bits than we need
    if (shift > size) {
        // mask unused bits
        r_val &= (1 << size) - 1;
    } else {
        // further bytes if required
        while (bits) {
            mask = (bits < 8) ? (0xff >> (8 - bits)) : 0xff;
            r_val += (p[byte++] & mask) << shift;
            shift += 8;
            bits -= (bits > 8) ? 8 : bits;
        }
    }

    if (is_signed) {
        // do sign expansion
        uint16_t sign_bit = 1 << (size - 1);
        if (r_val & sign_bit) {
            while (sign_bit) {
                r_val |= sign_bit;
                sign_bit <<= 1;
            }
        }
    }

    return (int16_t) r_val;
}

// check if the current report 
bool report_is_usable(uint16_t bit_count, uint8_t report_complete, Device *device) {
    printf("  - total bit count: %d (%d bytes, %d bits)\r\n",
           bit_count, bit_count / 8, bit_count % 8);

    device->report->report_size = bit_count / 8;

    // check if something useful was detected
    if (((device->report->type == REPORT_TYPE_JOYSTICK) &&
         ((report_complete & JOYSTICK_COMPLETE) == JOYSTICK_COMPLETE))) {
        printf("  - report %d is usable\r\n", device->report->report_id);
        return true;
    }

    printf("  - unusable report %d\r\n", device->report->report_id);
    return false;
}

bool parse_report_descriptor(uint8_t *rep, uint16_t rep_size, Device *device) {
    int8_t app_collection = 0;
    int8_t phys_log_collection = 0;
    uint8_t skip_collection = 0;
    int8_t generic_desktop = -1;   // depth at which first gen_desk was found
    uint8_t collection_depth = 0;

    uint8_t i;

    //
    uint8_t report_size, report_count;
    uint16_t bit_count = 0, usage_count = 0;
    uint16_t logical_minimum = 0, logical_maximum = 0;
    uint16_t physical_minimum = 0, physical_maximum = 0;
    memset(device->report, 0, sizeof(InputReportDescriptor));

    // mask used to check of all required components have been found, so
    // that e.g. both axes and the button of a joystick are ready to be used
    uint8_t report_complete = 0;

    // joystick/mouse components
    int8_t axis[MAX_AXIS];
    uint8_t buttons = 0;
    int8_t hat = -1;

    for (i = 0; i < MAX_AXIS; i++) axis[i] = -1;

    device->report->type = REPORT_TYPE_NONE;

    while (rep_size) {
        // extract short item
        uint8_t tag = ((item_t *) rep)->bTag;
        uint8_t type = ((item_t *) rep)->bType;
        uint8_t size = ((item_t *) rep)->bSize;

        rep++;
        rep_size--;   // one byte consumed

        uint32_t value = 0;
        if (size) {      // size 1/2/3
            value = *rep++;
            rep_size--;
        }

        if (size > 1) {  // size 2/3
            value = (value & 0xff) + ((uint32_t) (*rep++) << 8);
            rep_size--;
        }

        if (size > 2) {  // size 3
            value &= 0xffff;
            value |= ((uint32_t) (*rep++) << 16);
            value |= ((uint32_t) (*rep++) << 24);
            rep_size -= 2;
        }

        //    printf("Value = %d (%u)\n", value, value);

        // we are currently skipping an unknown/unsupported collection)
        if (skip_collection) {
            if (!type) {  // main item
                // any new collection increases the depth of collections to skip
                if (tag == 10) {
                    skip_collection++;
                    collection_depth++;
                }

                // any end collection decreases it
                if (tag == 12) {
                    skip_collection--;
                    collection_depth--;

                    // leaving the depth the generic desktop was valid for
                    if (generic_desktop > collection_depth)
                        generic_desktop = -1;
                }
            }

        } else {
            //      printf("-> Item tag=%d type=%d size=%d", tag, type, size);
            switch (type) {
                case 0:
                    // main item
                    switch (tag) {
                        case 8:
                            // handle found buttons
                            if (buttons) {
                                if ((device->report->type == REPORT_TYPE_JOYSTICK) ||
                                    (device->report->type == REPORT_TYPE_MOUSE)) {
                                    // scan for up to four buttons
                                    char b;
                                    for (b = 0; b < 12; b++) {
                                        if (report_count > b) {
                                            uint16_t this_bit = bit_count + b;

                                            printf("BUTTON%d @ %d (byte %d, mask %d)\r\n", b,
                                                   this_bit, this_bit / 8, 1 << (this_bit % 8));

                                            device->report->joystick.buttons[b].byte_offset = this_bit / 8;
                                            device->report->joystick.buttons[b].bitmask = 1 << (this_bit % 8);
                                        }
                                        device->report->joystick.button_count = report_count * report_size;
                                    }

                                    // we found at least one button which is all we want to accept this as a valid
                                    // joystick
                                    report_complete |= JOY_MOUSE_REQ_BTN_0;
                                    if (report_count > 1) report_complete |= JOY_MOUSE_REQ_BTN_1;
                                }
                            }

                            // handle found axes
                            char c;
                            for (c = 0; c < MAX_AXIS; c++) {
                                if (axis[c] >= 0) {
                                    uint16_t cnt = bit_count + report_size * axis[c];
                                    //printf("  (%c-AXIS @ %d (byte %d, bit %d))", 'X' + c,
                                    //       cnt, cnt / 8, cnt & 7);

                                    if ((device->report->type == REPORT_TYPE_JOYSTICK) ||
                                        (device->report->type == REPORT_TYPE_MOUSE)) {
                                        // save in joystick report
                                        device->report->joystick.axis[c].offset = cnt;
                                        device->report->joystick.axis[c].size = report_size;
                                        device->report->joystick.axis[c].logical.min = logical_minimum;
                                        device->report->joystick.axis[c].logical.max = logical_maximum;
                                        if (c == 0) report_complete |= JOY_MOUSE_REQ_AXIS_X;
                                        if (c == 1) report_complete |= JOY_MOUSE_REQ_AXIS_Y;
                                        printf("\r\nAXIS: {%i, %i, {%i, %i}\r\n",
                                               cnt, report_size, logical_minimum, logical_maximum);
                                    }
                                }
                            }

                            // handle found hat
                            if (hat >= 0) {
                                uint16_t cnt = bit_count + report_size * hat;
                                printf("  (HAT @ %d (byte %d, bit %d), size %d)\r\n",
                                       cnt, cnt / 8, cnt & 7, report_size);
                                if (device->report->type == REPORT_TYPE_JOYSTICK) {
                                    device->report->joystick.hat.offset = cnt;
                                    device->report->joystick.hat.size = report_size;
                                    device->report->joystick.hat.logical.min = logical_minimum;
                                    device->report->joystick.hat.logical.max = logical_maximum;
                                    device->report->joystick.hat.physical.min = physical_minimum;
                                    device->report->joystick.hat.physical.max = physical_maximum;
                                }
                            }

                            printf("INPUT(%d)\r\n", value);

                            // reset for next inputs
                            bit_count += report_count * report_size;
                            usage_count = 0;
                            buttons = 0;
                            for (i = 0; i < MAX_AXIS; i++) axis[i] = -1;
                            hat = -1;
                            break;

                        case 9:
                            printf("OUTPUT(%d)\r\n", value);
                            break;

                        case 11:
                            printf("FEATURE(%d)\r\n", value);
                            break;

                        case 10:
                            printf("COLLECTION(%d)", value);
                            collection_depth++;
                            usage_count = 0;

                            if (value == 1) {   // app collection
                                printf("  -> application\r\n");
                                app_collection++;
                            } else if (value == 0) {  // physical collection
                                printf("  -> physical\r\n");
                                phys_log_collection++;
                            } else if (value == 2) {  // logical collection
                                printf("  -> logical\r\n");
                                phys_log_collection++;
                            } else {
                                printf("skipping unsupported collection\r\n");
                                skip_collection++;
                            }
                            break;

                        case 12:
                            printf("END_COLLECTION(%d)\r\n", value);
                            collection_depth--;

                            // leaving the depth the generic desktop was valid for
                            if (generic_desktop > collection_depth)
                                generic_desktop = -1;

                            if (phys_log_collection) {
                                printf("  -> phys/log end\r\n");
                                phys_log_collection--;
                            } else if (app_collection) {
                                printf("  -> app end\r\n");
                                app_collection--;

                                // check if report is usable and stop parsing if it is
                                if (report_is_usable(bit_count, report_complete, device))
                                    return true;
                                else {
                                    // retry with next report
                                    bit_count = 0;
                                    report_complete = 0;
                                }

                            } else {
                                printf(" -> unexpected\r\n");
                                return false;
                            }
                            break;

                        default:
                            printf("unexpected main item %d\r\n", tag);
                            return false;
                            break;
                    }
                    break;

                case 1:
                    // global item
                    switch (tag) {
                        case 0:
                            printf("USAGE_PAGE(%d/0x%x)", value, value);
                            if (value == USAGE_PAGE_KEYBOARD) {
                                printf(" -> Keyboard\r\n");
                            } else if (value == USAGE_PAGE_GAMING) {
                                printf(" -> Game device\r\n");
                            } else if (value == USAGE_PAGE_LEDS) {
                                printf(" -> LEDs\r\n");
                            } else if (value == USAGE_PAGE_CONSUMER) {
                                printf(" -> Consumer\r\n");
                            } else if (value == USAGE_PAGE_BUTTON) {
                                printf(" -> Buttons\r\n");
                                buttons = 1;
                            } else if (value == USAGE_PAGE_GENERIC_DESKTOP) {
                                printf(" -> Generic Desktop\r\n");
                                if (generic_desktop < 0) generic_desktop = collection_depth;
                            } else {
                                printf(" -> UNSUPPORTED USAGE_PAGE\r\n");
                            }
                            break;

                        case 1:
                            printf("LOGICAL_MINIMUM(%d/%d)\r\n", value, (int16_t) value);
                            logical_minimum = value;
                            break;

                        case 2:
                            printf("LOGICAL_MAXIMUM(%d)\r\n", value);
                            logical_maximum = value;
                            break;

                        case 3:
                            printf("PHYSICAL_MINIMUM(%d/%d)\r\n", value, (int16_t) value);
                            physical_minimum = value;
                            break;

                        case 4:
                            printf("PHYSICAL_MAXIMUM(%d)\r\n", value);
                            physical_maximum = value;
                            break;

                        case 5:
                            printf("UNIT_EXPONENT(%d)\r\n", value);
                            break;

                        case 6:
                            printf("UNIT(%d)\r\n", value);
                            break;

                        case 7:
                            printf("REPORT_SIZE(%d)\r\n", value);
                            report_size = value;
                            break;

                        case 8:
                            printf("REPORT_ID(%d)\r\n", value);
                            device->report->report_id = value;
                            break;

                        case 9:
                            printf("REPORT_COUNT(%d)\r\n", value);
                            report_count = value;
                            break;

                        default:
                            printf("unexpected global item %d\r\n", tag);
                            return false;
                    }
                    break;

                case 2:
                    // local item
                    switch (tag) {
                        case 0:
                            // we only support mice, keyboards and joysticks
                            printf("USAGE(%d/0x%x)", value, value);
                            if (!collection_depth && (value == USAGE_KEYBOARD)) {
                                // usage(keyboard) is always allowed
                                printf(" -> Keyboard\r\n");
                                device->report->type = REPORT_TYPE_KEYBOARD;
                            } else if (!collection_depth && (value == USAGE_MOUSE)) {
                                // usage(mouse) is always allowed
                                printf(" -> Mouse\r\n");
                                device->report->type = REPORT_TYPE_MOUSE;
                            } else if (!collection_depth && ((value == USAGE_GAMEPAD) || (value == USAGE_JOYSTICK))) {
                                printf(" -> Gamepad/Joystick\r\n");
                                printf("Gamepad/Joystick usage found\r\n");
                                device->report->type = REPORT_TYPE_JOYSTICK;
                            } else if (value == USAGE_POINTER && app_collection) {
                                // usage(pointer) is allowed within the application collection
                                printf(" -> Pointer\r\n");
                            } else if (((value >= USAGE_X && value <= USAGE_RZ) || value == USAGE_WHEEL)
                                       && app_collection) {
                                // usage(x) and usage(y) are allowed within the app collection
                                printf(" -> axis usage\r\n");
                                // we support x and y axis on mice and joysticks (+wheel on mice)
                                if ((device->report->type == REPORT_TYPE_JOYSTICK) ||
                                    (device->report->type == REPORT_TYPE_MOUSE)) {
                                    if (value == USAGE_X) {
                                        printf("JOYSTICK/MOUSE: found x axis @ %d\r\n", usage_count);
                                        axis[0] = (int8_t) usage_count;
                                    }
                                    if (value == USAGE_Y) {
                                        printf("JOYSTICK/MOUSE: found y axis @ %d\r\n", usage_count);
                                        axis[1] = (int8_t) usage_count;
                                    }
                                    if (value == USAGE_Z) {
                                        printf("JOYSTICK/MOUSE: found z axis @ %d\r\n", usage_count);
                                        if (axis[2] == -1) axis[2] = (int8_t) usage_count; // don't override wheel
                                    }
                                    if (value == USAGE_RX || value == USAGE_RY || value == USAGE_RZ) {
                                        printf("JOYSTICK/MOUSE: found R%c axis @ %d\r\n",
                                               'X' + (value - USAGE_RX), usage_count);
                                        if (axis[3] == -1) axis[3] = (int8_t) usage_count;
                                    }
                                    if (value == USAGE_WHEEL) {
                                        printf("MOUSE: found wheel @ %d\r\n", usage_count);
                                        axis[2] = (int8_t) usage_count;
                                    }
                                }
                            } else if ((value == USAGE_HAT) && app_collection) {
                                // usage(hat) is allowed within the app collection
                                printf(" -> hat usage\r\n");
                                // we support hat on joysticks only
                                if (device->report->type == REPORT_TYPE_JOYSTICK) {
                                    printf("JOYSTICK: found hat @ %d\r\n", usage_count);
                                    hat = (int8_t) usage_count;
                                }
                            } else {
                                printf(" -> UNSUPPORTED USAGE\r\n");
                                //    return false;
                            }

                            usage_count++;
                            break;

                        case 1:
                            printf("USAGE_MINIMUM(%d)\r\n", value);
                            usage_count -= (value - 1);
                            break;

                        case 2:
                            printf("USAGE_MAXIMUM(%d)\r\n", value);
                            usage_count += value;
                            break;

                        default:
                            printf("unexpected local item %d\r\n", tag);
                            //  return false;
                            break;
                    }
                    break;

                default:
                    // reserved
                    printf("unexpected reserved item %d\r\n", tag);
                    // return false;
                    break;
            }
        }
    }

    // if we get here then no usable setup was found
    return false;
}
