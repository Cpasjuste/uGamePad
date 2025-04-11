//
// Created by cpasjuste on 10/04/25.
//

#include "pico_led.h"
#ifdef RGB_LED_PIN
#include "ws2812.pio.h"
#endif

using namespace uGamePad;

PicoLed::PicoLed() {
#ifdef RGB_LED_PIN
    pio_claim_free_sm_and_add_program_for_gpio_range(
        &ws2812_program, &pio, &sm, &offset, RGB_LED_PIN, 1, true);

    ws2812_program_init(pio, sm, offset, RGB_LED_PIN, 800000, false);
#endif
}

void PicoLed::put(const uint32_t pixel) {
#ifdef RGB_LED_PIN
    pio_sm_put_blocking(pio, sm, pixel << 8u);
#endif
}
