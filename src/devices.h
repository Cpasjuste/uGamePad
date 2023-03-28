//
// Created by cpasjuste on 28/03/23.
//

#ifndef UGAMEPAD_DEVICES_H
#define UGAMEPAD_DEVICES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gamepad_data.h"

const Device *find_device(uint16_t vid, uint16_t pid);

const Device *get_device(int i);

#ifdef __cplusplus
}
#endif

#endif //UGAMEPAD_DEVICES_H
