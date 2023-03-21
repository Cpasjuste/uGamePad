//
// Created by Kitsune on 9/11/2020.
//

#include "LED.h"

namespace LED {
  uint8_t blinks;
  uint8_t freq;
  uint8_t on;
  unsigned long next_blink;

  /**
   * Turns the LED on. This state is overwritten by blinking
   */
  void On() {
    on = true;
  }

  /**
   * Turns the LED off. This state is overwritten by blinking
   */
  void Off() {
    on = false;
  }

  /**
   * Blink {num} times, (rate) speed. Calling this again overwrites the number of blinks.
   * Also resets the blink timer. Always ends with the LED off even if the
   * LED was on before it was called.
   *
   * @param num Number of times to blink
   * @param speed of blink
   */
  void Blink(uint8_t num, uint8_t rate) {
    blinks = num;
    freq = rate;
    on = true;
    next_blink = millis() + freq;
  }

  /**
   * Updates the LED state.  Should be called during the main loop
   */
  void Update() {
    if (blinks && millis() >= next_blink) {
      if (on) blinks--;
      on = !on;
      next_blink = millis() + freq;
    }
    if (on) {
      digitalWrite(LED_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
};
