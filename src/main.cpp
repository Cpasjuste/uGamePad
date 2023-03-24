/*
 Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Arduino.h>
#include "main.h"
#include "game_controller.h"

// game controller wrapper
GameController *controller;

void setup() {
    // serial debug
    Debug.begin(115200);

    // led
    pinMode(LED_PIN, OUTPUT);

    /*
    // init output pins
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    //pinMode(A6, OUTPUT); // TX UART debug
    //pinMode(A7, OUTPUT); // RX UART debug
    pinMode(A8, OUTPUT);
    pinMode(A9, OUTPUT);
    pinMode(A10, OUTPUT);
    // SWCLK
    // pinMode(32, INPUT); // patch to use the SWCLK pin as output
    pinMode(32, OUTPUT);
    */

    controller = new GameController();

    // motd
    Debug.println("        _____                      _____          _ \r\n"
                  "       / ____|                    |  __ \\        | |\r\n"
                  " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
                  "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
                  "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
                  " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n");
}

void loop() {
    Led::Update();
    controller->update();

    if (controller->isConnected()) {
        if (controller->getButtonClick(A)) {
            Debug.println("rebooting...");
            Debug.flush();
            delay(1000);
            // reset to bootloader
            //*((volatile uint32_t *) (HMCRAMC0_ADDR + HMCRAMC0_SIZE - 4)) = 0xf01669ef;
            // normal reset
            NVIC_SystemReset();
        }
    }

    delay(1);
}
