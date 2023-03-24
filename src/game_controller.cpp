//
// Created by cpasjuste on 23/03/23.
//

#include <cstdint>
#include "tusb.h"
#include "main.h"
#include "game_controller.h"

extern GameControllerData xbox_controllers[];

GameController::GameController() {
    if (!tusb_init()) {
        Debug.println("tusb_init failed...");
    }

    if (!tuh_init(0)) {
        Debug.println("tuh_init failed...");
        while (true);
    }
}

void GameController::update() {
    if (tuh_inited()) tuh_task();
    else {
        Debug.println("oops, tuh_inited is false");
        while (true);
    }
}

bool GameController::isConnected() {
    // TODO
    return false;
}

uint8_t GameController::getButtonPress(uint8_t b) {
    // TODO
    return 0;
}

bool GameController::getButtonClick(uint8_t b) {
    // TODO
    return false;
}

GameControllerData *GameController::getData(uint16_t pid, uint16_t vid) {
    // lookup for xbox game controller
    int i = 0;
    int p = INT32_MAX;

    while (p != 0) {
        p = xbox_controllers[i].idProduct;
        if (p == pid && xbox_controllers[i].idVendor == vid) {
            return &xbox_controllers[i];
        }
        i++;
    }

    return nullptr;
}
