//
// Created by cpasjuste on 23/03/23.
//

#include <cstdint>
#include "tusb.h"
#include "main.h"
#include "game_controller.h"

extern PadData pads_data[];

GameController::GameController() {

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
