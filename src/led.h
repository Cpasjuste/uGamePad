#ifndef USBH_GAMEPAD_LED_H
#define USBH_GAMEPAD_LED_H

#include <Arduino.h>

#define LED_PIN LED_BUILTIN

namespace Led {
    const uint8_t BLINK_RATE_ERROR = 100;
    const uint8_t BLINK_RATE_SUCCESS = 255;

    void On();

    void Off();

    void Blink(uint8_t num, uint8_t rate);

    void Update();
};

#endif //USB2DB15_LED_H
