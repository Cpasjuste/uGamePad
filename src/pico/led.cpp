//
// Created by Kitsune on 9/11/2020.
//

#include "led.h"

using uGamePad::Led;

Led::Led() {
    pinMode(LED_PIN, OUTPUT);
}

void Led::On() {
    on = true;
}

void Led::Off() {
    on = false;
}

void Led::Blink(uint8_t num, uint8_t rate) {
    blinks = num + 1;
    freq = rate;
    on = true;
    next_blink = millis() + freq;
}

void Led::Update() {
    if (blinks && millis() >= next_blink) {
        if (on) blinks--;
        on = !on;
        next_blink = millis() + freq;
    }
    if (on) {
        digitalWrite(LED_PIN, LOW);
    } else {
        digitalWrite(LED_PIN, HIGH);
    }
}
