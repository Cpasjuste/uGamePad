//
// Created by cpasjuste on 24/03/23.
//

#include <valarray>
#include "main.h"
#include "gamepad.h"

using namespace uGamePad;

GamePad::GamePad() {
    // create an accurate map from ds4/5 analog inputs (0 to 255) to uGamePad metrics (-32768 to 32767)
    for (int i = 0; i < 128; i++) {
        float t = (float) i / 127.0f;
        m_analog_map[i + 128] = bezierY(t);
        m_analog_map[127 - i] = -1 * m_analog_map[i + 128];
    }
}

void GamePad::setDevice(const Device *device, uint8_t dev_addr, uint8_t instance) {
    p_device = device;
    m_addr = dev_addr;
    m_instance = instance;
    printf("new gamepad discovered: %s (%04x:%04x)\r\n",
           device->name, p_device->vendor, p_device->product);
}

uint16_t GamePad::getButtonsFromHat(int hat) {
    if (hat != 0x0000007f) printf("hat: %08x\r\n", hat);
    uint16_t buttons = 0;

    static constexpr uint16_t table[] = {
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

    if (type & ReportData::AxisType::AXIS_UI8) {
        analogX = (float) m_analog_map[x];
        analogY = (float) m_analog_map[y];
    }

    if (type & ReportData::AxisType::AXIS_FLIP_Y) {
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

void GamePad::lerp(Utility::Vec2i *dest, Utility::Vec2i *first, Utility::Vec2i *second, float t) {
    dest->x = (int16_t) ((float) first->x + ((float) second->x - (float) first->x) * t);
    dest->y = (int16_t) ((float) first->y + ((float) second->y - (float) first->y) * t);
}

int GamePad::bezierY(float t) {
    Utility::Vec2i ab, bc, cd, ab_bc, bc_cd, dest;
    lerp(&ab, &m_pa, &m_pb, t);     /* point between a and b */
    lerp(&bc, &m_pb, &m_pc, t);     /* point between b and c */
    lerp(&cd, &m_pc, &m_pd, t);     /* point between c and d */
    lerp(&ab_bc, &ab, &bc, t);       /* point between ab and bc */
    lerp(&bc_cd, &bc, &cd, t);       /* point between bc and cd */
    lerp(&dest, &ab_bc, &bc_cd, t);   /* point on the bezier-curve */
    return dest.y;
}

GamePad::Output *GamePad::getOutputMode() {
    for (auto &out: m_outputModes) {
        if (out.mode == m_outputMode) {
            return &out;
        }
    }

    return nullptr;
}

void GamePad::setOutputMode(const GamePad::Mode &mode) {
    for (const auto &out: m_outputModes) {
        if (out.mode == mode) {
            m_outputMode = mode;
            printf("GamePad::setOutputMode: %s\r\n", getOutputMode()->name.c_str());
            break;
        }
    }
}

void GamePad::setOutputMode(const std::string &modeName) {
    for (const auto &out: m_outputModes) {
        if (out.name == modeName) {
            m_outputMode = out.mode;
            printf("GamePad::setOutputMode: %s\r\n", getOutputMode()->name.c_str());
            break;
        }
    }
}

void GamePad::loop() {
    // handle auto-repeat
    if (m_repeatClock.getElapsedTime().asMilliseconds() >= m_repeatDelayMs) {
        m_repeatClock.restart();
        m_buttons_prev = m_buttons;
    } else {
        uint16_t diff = m_buttons_prev ^ m_buttons;
        m_buttons_prev = m_buttons;
        if (diff > 0) {
            m_repeatClock.restart();
        } else {
            m_buttons = Button::DELAY;
        }
    }
}
