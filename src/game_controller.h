//
// Created by cpasjuste on 22/03/23.
//

#ifndef USBH_GAMEPAD_GAME_CONTROLLER_H
#define USBH_GAMEPAD_GAME_CONTROLLER_H

#include "game_controller_desc.h"
#include "controller_enums.h"
#include "UsbHost/Usb.h"

class GameController {
public:
    GameController();

    void update();

    bool isConnected();

    uint8_t getButtonPress(ButtonEnum b);

    bool getButtonClick(ButtonEnum b);

    static GameControllerDesc *getController(uint16_t pid, uint16_t vid);

private:
    USBHost p_usb_host;
};

#endif //USBH_GAMEPAD_GAME_CONTROLLER_H
