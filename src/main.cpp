/*
 Example sketch for the Xbox 360 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "gamepad.h"

uGamePad::State pad;
uGamePad::State pad_old;

void setup() {
    // serial debug
#if defined(ARDUINO_RASPBERRY_PI_PICO)
    Debug.setTX(D16);
    Debug.setRX(D17);
#endif
    Debug.begin(115200);

    // led
    pinMode(LED_PIN, OUTPUT);

    // init output pins
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);
    pinMode(D9, OUTPUT);
    pinMode(D10, OUTPUT);
    pinMode(D11, OUTPUT);

    // tinyusb
    if (!tusb_init()) {
        printf("tusb_init failed...\r\n");
    }

    if (!tuh_init(0)) {
        printf("tuh_init failed...\r\n");
        while (true);
    }

    // motd
    printf("        _____                      _____          _ \r\n"
           "       / ____|                    |  __ \\        | |\r\n"
           " _   _| |  __  __ _ _ __ ___   ___| |__) |_ _  __| |\r\n"
           "| | | | | |_ |/ _` | '_ ` _ \\ / _ \\  ___/ _` |/ _` |\r\n"
           "| |_| | |__| | (_| | | | | | |  __/ |  | (_| | (_| |\r\n"
           " \\__,_|\\_____|\\__,_|_| |_| |_|\\___|_|   \\__,_|\\__,_|\r\n\n");
}

void loop() {
    // handle usb host updates
    if (tuh_inited()) {
        tuh_task();
    } else {
        Debug.println("oops, tinyusb host service not inited...");
        while (true);
    }

    // handle led updates
    Led::Update();

    // get gamepad sate
    pad = uGamePad::getState();
    // only update on button change
    bool changed = pad_old.buttons ^ pad.buttons;
    pad_old.buttons = pad.buttons;
    if (changed) {
        uGamePad::PinMapping *mapping = uGamePad::getPinMapping();
        // generate pin output
        for (int i = 0; i < MAX_BUTTONS; i++) {
            if (mapping[i].pin == D6 || mapping[i].pin == D7 || mapping[i].pin == SWCLK) {
                // TODO: handle swd debug pins / buttons config change
                continue;
            }
            digitalWrite(mapping[i].pin, pad.buttons & mapping[i].button ? HIGH : LOW);
        }
        //printf("buttons: %hx\r\n", pad.buttons);
    }

    delay(1);
}
