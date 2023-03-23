//
// Created by cpasjuste on 23/03/23.
//

#include <cstdint>
#include "main.h"
#include "game_controller.h"
#include "xbox_usb.h"

USBHost m_usb_host;
XboxUsb m_xbox(&m_usb_host);
extern GameControllerData xbox_controllers[];

GameController::GameController() {
    if (m_usb_host.Init()) {
        Debug.println("GameController: usb host library error...");
        Led::Blink(10, Led::BLINK_RATE_ERROR);
        while (true); // halt
    }
}

void GameController::update() {
    m_usb_host.Task();
}

bool GameController::isConnected() {
    if (m_xbox.Xbox360Connected) return true;
    return false;
}

uint8_t GameController::getButtonPress(ButtonEnum b) {
    if (m_xbox.Xbox360Connected) return m_xbox.getButtonPress(b);
    return 0;
}

bool GameController::getButtonClick(ButtonEnum b) {
    if (m_xbox.Xbox360Connected) return m_xbox.getButtonClick(b);
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
