#ifndef USBH_GAMEPAD_LED_H
#define USBH_GAMEPAD_LED_H

#include <Arduino.h>

#define LED_PIN LED_BUILTIN

namespace uGamePad {
    class Led {
    public:
        const uint8_t BLINK_RATE_ERROR = 100;
        const uint8_t BLINK_RATE_SUCCESS = 255;

        Led();

        void On();

        void Off();

        void Blink(uint8_t num, uint8_t rate);

        void Update();

    private:
        uint8_t blinks;
        uint8_t freq;
        uint8_t on = false;
        unsigned long next_blink;
    };
}

#endif //USBH_GAMEPAD_LED_H
