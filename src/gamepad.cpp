//
// Created by cpasjuste on 24/03/23.
//

#include "main.h"
#include "gamepad.h"

using namespace uGamePad;

GamePad::GamePad() {
    // create an accurate map from ds4/5 analog inputs (0 to 255) to uGamePad metrics (-32768 to 32767)
    for (int i = 0; i < 128; i++) {
        const float t = static_cast<float>(i) / 127.0f;
        m_analog_map[i + 128] = bezierY(t);
        m_analog_map[127 - i] = -1 * m_analog_map[i + 128];
    }
}

uint32_t GamePad::getButtonsFromHat(const int hat) {
    uint32_t buttons = 0;

    static constexpr uint32_t table[] = {
        DPAD_UP,
        DPAD_UP | DPAD_RIGHT,
        DPAD_RIGHT,
        DPAD_DOWN | DPAD_RIGHT,
        DPAD_DOWN,
        DPAD_LEFT | DPAD_DOWN,
        DPAD_LEFT,
        DPAD_LEFT | DPAD_UP,
    };

    const auto i = static_cast<int>(hat);
    if (i < 8) {
        buttons |= table[i];
    }

    return buttons;
}

uint32_t GamePad::getButtonsFromAxis(const int x, const int y, const uint8_t type) const {
    uint32_t buttons = 0;
    auto analogX = (float) x, analogY = (float) y;

    if (type & AXIS_TYPE_U8) {
        analogX = (float) m_analog_map[x];
        analogY = (float) m_analog_map[y];
    }

    if (type & AXIS_TYPE_FLIP_Y) {
        analogY = -analogY;
    }

    if (std::sqrt(analogX * analogX + analogY * analogY) >= DEAD_ZONE) {
        constexpr float slope = 0.414214f;
        // symmetric angular zones for all eight digital directions
        if (analogY > 0 && analogX > 0) {
            // upper right quadrant
            if (analogY > slope * analogX)
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_UP : Button::AXIS_R_UP;
            if (analogX > slope * analogY)
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_RIGHT : Button::AXIS_R_RIGHT;
        } else if (analogY > 0 && analogX <= 0) {
            // upper left quadrant
            if (analogY > slope * (-analogX))
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_UP : Button::AXIS_R_UP;
            if ((-analogX) > slope * analogY)
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_LEFT : Button::AXIS_R_LEFT;
        } else if (analogY <= 0 && analogX > 0) {
            // lower right quadrant
            if ((-analogY) > slope * analogX)
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_DOWN : Button::AXIS_R_DOWN;
            if (analogX > slope * (-analogY))
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_RIGHT : Button::AXIS_R_RIGHT;
        } else if (analogY <= 0 && analogX <= 0) {
            // lower left quadrant
            if ((-analogY) > slope * (-analogX))
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_DOWN : Button::AXIS_R_DOWN;
            if ((-analogX) > slope * (-analogY))
                buttons |= type & AXIS_TYPE_LEFT ? Button::AXIS_L_LEFT : Button::AXIS_R_LEFT;
        }
    }

    return buttons;
}

uint8_t GamePad::getButtonIndex(const uint32_t button) {
    for (int i = 0; i < MAX_BUTTONS; i++) {
        if (button & BIT(i)) return i;
    }

    return UINT8_MAX;
}

void GamePad::lerp(Utility::Vec2i *dest, Utility::Vec2i *first, Utility::Vec2i *second, float t) {
    dest->x = (int16_t) ((float) first->x + ((float) second->x - (float) first->x) * t);
    dest->y = (int16_t) ((float) first->y + ((float) second->y - (float) first->y) * t);
}

int GamePad::bezierY(float t) {
    Utility::Vec2i ab, bc, cd, ab_bc, bc_cd, dest;
    lerp(&ab, &m_pa, &m_pb, t); /* point between a and b */
    lerp(&bc, &m_pb, &m_pc, t); /* point between b and c */
    lerp(&cd, &m_pc, &m_pd, t); /* point between c and d */
    lerp(&ab_bc, &ab, &bc, t); /* point between ab and bc */
    lerp(&bc_cd, &bc, &cd, t); /* point between bc and cd */
    lerp(&dest, &ab_bc, &bc_cd, t); /* point on the bezier-curve */
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

void GamePad::flush() {
    // disable "DELAY"
    const auto delay = m_repeatDelayMs;
    m_repeatDelayMs = 0;

    while (m_buttons) {
        m_buttons = 0;
        getPlatform()->getHid()->loop();
        getPlatform()->getPad()->loop();
    }

    m_repeatDelayMs = delay;
}

bool GamePad::onHidReport(const uint8_t *report, const uint16_t len) {
    if (!p_device || !p_device->report) {
        printf("uGamePad::onHidReport: error: device not set\r\n");
        return false;
    }

    //printf("uGamePad::loop: received data for '%s', len: %i)\r\n", p_device->name, len);
    const auto *data = p_device->report;

    // process axis // TODO: fix
    if (data->is_xbox) {
        // "xinput" gamepad
        for (int i = 0; i < MAX_AXIS; i += 2) {
            if (data->joystick.axis[i].offset >= len * 8) continue;
            if (data->joystick.axis[i].size > 8) {
                const int16_t x = (int16_t &) report[data->joystick.axis[i].offset / 8];
                const int16_t y = (int16_t &) report[data->joystick.axis[i + 1].offset / 8];
                m_buttons |= getButtonsFromAxis(x, y, AXIS_TYPE_S16 | (i < 2 ? AXIS_TYPE_LEFT : AXIS_TYPE_RIGHT));
            } else {
                const uint8_t x = (uint8_t &) report[data->joystick.axis[i].offset / 8];
                const uint8_t y = (uint8_t &) report[data->joystick.axis[i + 1].offset / 8];
                m_buttons |= getButtonsFromAxis(x, y, AXIS_TYPE_U8 | (i < 2 ? AXIS_TYPE_LEFT : AXIS_TYPE_RIGHT));
            }
        }
    } else {
        // generic hid gamepad
        int16_t axis[MAX_AXIS];
        for (int i = 0; i < MAX_AXIS; i++) {
            const bool is_signed = data->joystick.axis[i].logical.min > data->joystick.axis[i].logical.max;
            if (data->joystick.axis[i].size == 0) {
                axis[i] = 127;
            } else {
                axis[i] = parse_joystick_bits(report, data->joystick.axis[i].offset,
                                              data->joystick.axis[i].size, is_signed);
                uint16_t min = data->joystick.axis[i].logical.min;
                uint16_t max = data->joystick.axis[i].logical.max;
                if (min > max) {
                    // signed -> unsigned
                    // FIXME: do proper sign extension based on bSize of min and max in the report
                    if (min > 255 || max > 255) {
                        // assume 16 bit values
                        min += 32768;
                        max += 32768;
                        axis[i] += 32767;
                    } else {
                        // assume 8 bit values
                        min = (min + 128) & 0xff;
                        max = (max + 128) & 0xff;
                        axis[i] = (int16_t) (((axis[i] & 0xff) + 128) & 0xff);
                    }
                }

                const int h_range = (max - min);
                // scale to 0-255
                if (axis[i] <= min) axis[i] = (int16_t) min;
                else if (axis[i] >= max) axis[i] = (int16_t) max;
                if (!h_range) axis[i] = 127;
                else axis[i] = (int16_t) (((axis[i] - min) * 255) / h_range);
            }
        }

        for (int i = 0; i < MAX_AXIS; i += 2) {
            if (data->joystick.axis[i].size == 0) continue;
            m_buttons |= getButtonsFromAxis(
                axis[i], axis[i + 1], AXIS_TYPE_U8 | AXIS_TYPE_FLIP_Y | (i < 2 ? AXIS_TYPE_LEFT : AXIS_TYPE_RIGHT));
        }

        // hat
        if (data->joystick.hat.size) {
            m_buttons |= getButtonsFromHat(report[data->joystick.hat.offset / 8]);
        }
    }

    // process buttons
    for (int i = 0; i < data->joystick.button_count; i++) {
        if (data->joystick.buttons[i].byte_offset == INPUT_DUMMY) continue;
        m_buttons |= report[data->joystick.buttons[i].byte_offset] & data->joystick.buttons[i].bitmask ? (1 << i) : 0;
    }

#ifndef NDEBUG
    if (m_buttons) {
        //printf("%s: %s\r\n", p_device->name, Utility::toString(m_buttons).c_str());
    }
#endif

    return true;
}

void GamePad::loop() {
    // handle auto-repeat for ui stuff
    if (m_repeatClock.getElapsedTime().asMilliseconds() >= m_repeatDelayMs) {
        m_repeatClock.restart();
        m_buttons_prev = m_buttons;
    } else {
        const uint32_t diff = m_buttons_prev ^ m_buttons;
        m_buttons_prev = m_buttons;
        if (diff) {
            m_repeatClock.restart();
        } else {
            m_buttons = DELAY;
        }
    }
}
