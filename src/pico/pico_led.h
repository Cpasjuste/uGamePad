//
// Created by cpasjuste on 10/04/25.
//

#ifndef PICO_LED_H
#define PICO_LED_H

#include <sys/types.h>
#include <hardware/pio.h>
#include "led.h"

#define RGB_LED_PIN 23

namespace uGamePad {
    class PicoLed final : public Led {
    public:
        PicoLed();

        void put(uint32_t pixel) override;

    private:
        PIO pio = nullptr;
        uint sm = 0;
        uint offset = 0;
    };
}

#endif //PICO_LED_H
