//
// Created by cpasjuste on 23/03/23.
//

#ifndef USBH_GAMEPAD_GAME_CONTROLLER_DATA_H
#define USBH_GAMEPAD_GAME_CONTROLLER_DATA_H

#define TYPE_UNKNOWN     0
#define TYPE_XBOX        1
#define TYPE_XBOX360     2
#define TYPE_XBOX360W    3
#define TYPE_XBOXONE     4
#define TYPE_DS4         5
#define TYPE_DS5         6

typedef struct PD {
    uint16_t idVendor;
    uint16_t idProduct;
    char *name;
    uint8_t mapping;
    uint8_t type;
} PadData;

#endif //USBH_GAMEPAD_GAME_CONTROLLER_DATA_H
