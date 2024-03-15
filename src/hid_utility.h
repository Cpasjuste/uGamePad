#ifndef U_GAMEPAD_HID_UTILITY_H
#define U_GAMEPAD_HID_UTILITY_H

#include "devices.h"

bool parse_report_descriptor(uint8_t *rep, uint16_t rep_size, uGamePad::Device *conf);

// collect bits from byte stream and assemble them into a signed word
int16_t parse_joystick_bits(const uint8_t *p, uint16_t offset, uint8_t size, bool is_signed);

#endif // U_GAMEPAD_HID_UTILITY_H
