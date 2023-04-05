//
// Created by cpasjuste on 24/03/23.
//

#include <cstdio>
#include <complex>
#include <Arduino.h>
#include "gamepad.h"

#define printf Serial1.printf

using namespace uGamePad;

GamePad::GamePad() {
    // create an accurate map from ds4/5 analog inputs (0 to 255) to uGamePad metrics (-32768 to 32767)
    for (int i = 0; i < 128; i++) {
        float t = (float) i / 127.0f;
        m_analog_map[i + 128] = calc_bezier_y(t);
        m_analog_map[127 - i] = -1 * m_analog_map[i + 128];
    }
}

void GamePad::setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance) {
    printf("new gamepad discovered: %s\r\n", device->name);
    p_device = device;
    m_addr = dev_addr;
    m_instance = instance;
}

uint16_t GamePad::getButtonsFromHat(int hat) {
    uint16_t buttons = 0;
    static constexpr int table[] = {
            Button::UP,
            Button::UP | Button::RIGHT,
            Button::RIGHT,
            Button::DOWN | Button::RIGHT,
            Button::DOWN,
            Button::LEFT | Button::DOWN,
            Button::LEFT,
            Button::LEFT | Button::UP,
    };
    auto i = static_cast<int>(hat);
    if (i < 8) {
        buttons |= table[i];
    }

    return buttons;
}

uint16_t GamePad::getButtonsFromAxis(int x, int y, uint8_t type) {
    uint16_t buttons = 0;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones
    auto analogX = (float) x, analogY = (float) y;

    if (type & Report::AxisType::AXIS_UI8) {
        analogX = (float) m_analog_map[x];
        analogY = (float) m_analog_map[y];
    }

    if (type & Report::AxisType::AXIS_FLIP_Y) {
        analogY = -analogY;
    }

    if (std::sqrt(analogX * analogX + analogY * analogY) >= DEAD_ZONE) {
        // symmetric angular zones for all eight digital directions
        if (analogY > 0 && analogX > 0) {
            // upper right quadrant
            if (analogY > slope * analogX)
                buttons |= Button::UP;
            if (analogX > slope * analogY)
                buttons |= Button::RIGHT;
        } else if (analogY > 0 && analogX <= 0) {
            // upper left quadrant
            if (analogY > slope * (-analogX))
                buttons |= Button::UP;
            if ((-analogX) > slope * analogY)
                buttons |= Button::LEFT;
        } else if (analogY <= 0 && analogX > 0) {
            // lower right quadrant
            if ((-analogY) > slope * analogX)
                buttons |= Button::DOWN;
            if (analogX > slope * (-analogY))
                buttons |= Button::RIGHT;
        } else if (analogY <= 0 && analogX <= 0) {
            // lower left quadrant
            if ((-analogY) > slope * (-analogX))
                buttons |= Button::DOWN;
            if ((-analogX) > slope * (-analogY))
                buttons |= Button::LEFT;
        }
    }

    return buttons;
}

void GamePad::lerp(GamePad::point *dest, GamePad::point *first, GamePad::point *second, float t) {
    dest->x = (int) ((float) first->x + ((float) second->x - (float) first->x) * t);
    dest->y = (int) ((float) first->y + ((float) second->y - (float) first->y) * t);
}

int GamePad::calc_bezier_y(float t) {
    point ab, bc, cd, ab_bc, bc_cd, dest;
    lerp(&ab, &m_pa, &m_pb, t);     /* point between a and b */
    lerp(&bc, &m_pb, &m_pc, t);     /* point between b and c */
    lerp(&cd, &m_pc, &m_pd, t);     /* point between c and d */
    lerp(&ab_bc, &ab, &bc, t);       /* point between ab and bc */
    lerp(&bc_cd, &bc, &cd, t);       /* point between bc and cd */
    lerp(&dest, &ab_bc, &bc_cd, t);   /* point on the bezier-curve */
    return dest.y;
}
