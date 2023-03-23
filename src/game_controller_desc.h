//
// Created by cpasjuste on 23/03/23.
//

#ifndef USBH_GAMEPAD_GAME_CONTROLLER_DESC_H
#define USBH_GAMEPAD_GAME_CONTROLLER_DESC_H

#define XTYPE_XBOX        0
#define XTYPE_XBOX360     1
#define XTYPE_XBOX360W    2
#define XTYPE_XBOXONE     3
#define XTYPE_UNKNOWN     4

typedef struct GC {
    uint16_t idVendor;
    uint16_t idProduct;
    char *name;
    uint8_t mapping;
    uint8_t xtype;
    uint8_t packet_type;
} GameControllerDesc;

#endif //USBH_GAMEPAD_GAME_CONTROLLER_DESC_H
