//
// Created by cpasjuste on 22/03/23.
//

#ifndef USBH_GAMEPAD_GAME_CONTROLLER_H
#define USBH_GAMEPAD_GAME_CONTROLLER_H

#include "game_controller_data.h"

class GameController {
public:
    GameController();

    void update();

    bool isConnected();

    uint8_t getButtonPress(uint8_t b);

    bool getButtonClick(uint8_t b);

    static GamepadData *getData(uint16_t pid, uint16_t vid);
};

#endif //USBH_GAMEPAD_GAME_CONTROLLER_H
