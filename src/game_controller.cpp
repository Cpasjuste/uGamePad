//
// Created by cpasjuste on 23/03/23.
//

#include <cstdint>
#include "game_controller.h"
#include "xbox_usb.h"
#include "led.h"

XboxUsb *Xbox;
extern GameControllerDesc xbox_controllers[];

GameController::GameController() {
    if (p_usb_host.Init()) {
        Serial1.println("GameController: usb host library error...");
        Led::Blink(10, Led::BLINK_RATE_ERROR);
        while (true); // halt
    }

    Xbox = new XboxUsb(&p_usb_host);
}

void GameController::update() {
    p_usb_host.Task();
}

bool GameController::isConnected() {
    if (Xbox->Xbox360Connected) return true;
    return false;
}

uint8_t GameController::getButtonPress(ButtonEnum b) {
    if (Xbox->Xbox360Connected) return Xbox->getButtonPress(b);
    return 0;
}

bool GameController::getButtonClick(ButtonEnum b) {
    if (Xbox->Xbox360Connected) return Xbox->getButtonClick(b);
    return false;
}

GameControllerDesc *GameController::getController(uint16_t pid, uint16_t vid) {
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
