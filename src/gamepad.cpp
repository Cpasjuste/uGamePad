//
// Created by cpasjuste on 24/03/23.
//

#include <Arduino.h>
#include "tusb.h"
#include "gamepad.h"
#include "utility.h"

static uGamePad::PinMapping padMapping[MAX_BUTTONS] = {
        {uGamePad::Button::B_1,      D2},
        {uGamePad::Button::B_2,      D1},
        {uGamePad::Button::B_3,      D0},
        {uGamePad::Button::B_4,      D10},
        {uGamePad::Button::B_5,      D8},
        {uGamePad::Button::B_6,      D7},
        {uGamePad::Button::B_SELECT, D11},
        {uGamePad::Button::B_START,  D9},
        {uGamePad::Button::B_UP,     D5},
        {uGamePad::Button::B_DOWN,   D6},
        {uGamePad::Button::B_LEFT,   D4},
        {uGamePad::Button::B_RIGHT,  D3},
};

uGamePad::uGamePad() {
    // create an accurate map from ds4/5 analog inputs (0 to 255) to uGamePad metrics (-32768 to 32767)
    for (int i = 0; i < 128; i++) {
        float t = (float) i / 127.0f;
        m_analog_map[i + 128] = calc_bezier_y(t);
        m_analog_map[127 - i] = -1 * m_analog_map[i + 128];
    }
}

void uGamePad::setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance) {
    TU_LOG1("new gamepad discovered: %s\r\n", device->name);
    p_device = device;
    m_addr = dev_addr;
    m_instance = instance;
}

void uGamePad::setLed(uint8_t type) {
    // TODO: handle type, ds4, etc...
    if (p_device && p_device->type == TYPE_XBOX360) {
        uint8_t msg[3] = {0x01, 0x03, 0x02};
        tuh_hid_set_report(m_addr, m_instance, 5, HID_REPORT_TYPE_OUTPUT, &msg, 3);
    }
}

bool uGamePad::update(const uint8_t *report, uint16_t len) {
    if (!p_device) {
        TU_LOG1("uGamePad::update: error: device not set\r\n");
        return false;
    }

    TU_LOG2("uGamePad::update: received report for '%s': type: %i, len: %i)\r\n",
            p_device->name, p_device->type, len);

    if (p_device->type == TYPE_XBOX360) {
        if (sizeof(XBOXReport) == len && report[0] == 0x00) {
            auto r = reinterpret_cast<const XBOXReport *>(report);
            m_buttons =
                    (r->buttons & XBOXReport::Button::A ? uGamePad::Button::B_1 : 0) |
                    (r->buttons & XBOXReport::Button::B ? uGamePad::Button::B_2 : 0) |
                    (r->buttons & XBOXReport::Button::X ? uGamePad::Button::B_3 : 0) |
                    (r->buttons & XBOXReport::Button::Y ? uGamePad::Button::B_4 : 0) |
                    (r->buttons & XBOXReport::Button::LB ? uGamePad::Button::B_5 : 0) |
                    (r->buttons & XBOXReport::Button::RB ? uGamePad::Button::B_6 : 0) |
                    (r->buttons & XBOXReport::Button::PAD_UP ? uGamePad::Button::B_UP : 0) |
                    (r->buttons & XBOXReport::Button::PAD_DOWN ? uGamePad::Button::B_DOWN : 0) |
                    (r->buttons & XBOXReport::Button::PAD_LEFT ? uGamePad::Button::B_LEFT : 0) |
                    (r->buttons & XBOXReport::Button::PAD_RIGHT ? uGamePad::Button::B_RIGHT : 0) |
                    (r->buttons & XBOXReport::Button::BACK ? uGamePad::Button::B_SELECT : 0) |
                    (r->buttons & XBOXReport::Button::START ? uGamePad::Button::B_START : 0);
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1]);
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1]);
            if (m_buttons != 0) TU_LOG1("x360: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("uGamePad::update: skipping report, wrong packet (xbox)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS4) {
        if (sizeof(DS4Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS4Report *>(report);
            m_buttons =
                    (r->buttons1 & DS4Report::Button1::CROSS ? uGamePad::Button::B_1 : 0) |
                    (r->buttons1 & DS4Report::Button1::CIRCLE ? uGamePad::Button::B_2 : 0) |
                    (r->buttons1 & DS4Report::Button1::SQUARE ? uGamePad::Button::B_3 : 0) |
                    (r->buttons1 & DS4Report::Button1::TRIANGLE ? uGamePad::Button::B_4 : 0) |
                    (r->buttons2 & DS4Report::Button2::L1 ? uGamePad::Button::B_5 : 0) |
                    (r->buttons2 & DS4Report::Button2::R1 ? uGamePad::Button::B_6 : 0) |
                    (r->buttons2 & DS4Report::Button2::SHARE ? uGamePad::Button::B_SELECT : 0) |
                    (r->t_pad ? uGamePad::Button::B_SELECT : 0) |
                    (r->buttons2 & DS4Report::Button2::OPTIONS ? uGamePad::Button::B_START : 0);
            // axis
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1], AXIS_255 | AXIS_FLIP_Y);
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1], AXIS_255 | AXIS_FLIP_Y);
            // hat
            m_buttons |= uGamePad::getButtonsFromHat(r->buttons1 & 15);
            if (m_buttons != 0) TU_LOG1("ds4: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("uGamePad::update: skipping report, wrong packet (ds4)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS5) {
        if (sizeof(DS5Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS5Report *>(report);
            auto buttons = r->buttons[0] | (r->buttons[1] << 8) | (r->buttons[2] << 16);
            m_buttons =
                    (buttons & DS4Report::Button1::CROSS ? uGamePad::Button::B_1 : 0) |
                    (buttons & DS4Report::Button1::CIRCLE ? uGamePad::Button::B_2 : 0) |
                    (buttons & DS4Report::Button1::TRIANGLE ? uGamePad::Button::B_3 : 0) |
                    (buttons & DS4Report::Button1::SQUARE ? uGamePad::Button::B_4 : 0) |
                    (buttons & DS4Report::Button2::L1 ? uGamePad::Button::B_5 : 0) |
                    (buttons & DS4Report::Button2::R1 ? uGamePad::Button::B_6 : 0) |
                    (buttons & DS4Report::Button2::SHARE ? uGamePad::Button::B_SELECT : 0) |
                    (buttons & DS4Report::Button2::OPTIONS ? uGamePad::Button::B_START : 0);
            // axis
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1], AXIS_255 | AXIS_FLIP_Y);
            m_buttons |= uGamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1], AXIS_255 | AXIS_FLIP_Y);
            // hat
            m_buttons |= uGamePad::getButtonsFromHat(r->buttons[0] & 15);
            if (m_buttons != 0) TU_LOG1("ds5: %s\r\n", Utility::toString(m_buttons).c_str());
        } else {
            TU_LOG2("tuh_hid_report_received_cb: skipping report, wrong packet (ds5)\r\n");
            return false;
        }
    } else {
        return false;
    }

    return true;
}

uint16_t uGamePad::getButtonsFromHat(int hat) {
    uint16_t buttons = 0;
    static constexpr int table[] = {
            Button::B_UP,
            Button::B_UP | Button::B_RIGHT,
            Button::B_RIGHT,
            Button::B_DOWN | Button::B_RIGHT,
            Button::B_DOWN,
            Button::B_LEFT | Button::B_DOWN,
            Button::B_LEFT,
            Button::B_LEFT | Button::B_UP,
    };
    auto i = static_cast<int>(hat);
    if (i < 8) {
        buttons |= table[i];
    }

    return buttons;
}

uint16_t uGamePad::getButtonsFromAxis(int x, int y, uint8_t type) {
    uint16_t buttons = 0;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones
    auto analogX = (float) x, analogY = (float) y;

    if (type & AXIS_255) {
        analogX = (float) m_analog_map[x];
        analogY = (float) m_analog_map[y];
    }

    if (type & AXIS_FLIP_Y) {
        analogY = -analogY;
    }

    if (std::sqrt(analogX * analogX + analogY * analogY) >= DEAD_ZONE) {
        // symmetric angular zones for all eight digital directions
        if (analogY > 0 && analogX > 0) {
            // upper right quadrant
            if (analogY > slope * analogX)
                buttons |= Button::B_UP;
            if (analogX > slope * analogY)
                buttons |= Button::B_RIGHT;
        } else if (analogY > 0 && analogX <= 0) {
            // upper left quadrant
            if (analogY > slope * (-analogX))
                buttons |= Button::B_UP;
            if ((-analogX) > slope * analogY)
                buttons |= Button::B_LEFT;
        } else if (analogY <= 0 && analogX > 0) {
            // lower right quadrant
            if ((-analogY) > slope * analogX)
                buttons |= Button::B_DOWN;
            if (analogX > slope * (-analogY))
                buttons |= Button::B_RIGHT;
        } else if (analogY <= 0 && analogX <= 0) {
            // lower left quadrant
            if ((-analogY) > slope * (-analogX))
                buttons |= Button::B_DOWN;
            if ((-analogX) > slope * (-analogY))
                buttons |= Button::B_LEFT;
        }
    }

    return buttons;
}

void uGamePad::lerp(uGamePad::point *dest, uGamePad::point *first, uGamePad::point *second, float t) {
    dest->x = (int) ((float) first->x + ((float) second->x - (float) first->x) * t);
    dest->y = (int) ((float) first->y + ((float) second->y - (float) first->y) * t);
}

int uGamePad::calc_bezier_y(float t) {
    point ab, bc, cd, ab_bc, bc_cd, dest;
    lerp(&ab, &m_pa, &m_pb, t);     /* point between a and b */
    lerp(&bc, &m_pb, &m_pc, t);     /* point between b and c */
    lerp(&cd, &m_pc, &m_pd, t);     /* point between c and d */
    lerp(&ab_bc, &ab, &bc, t);       /* point between ab and bc */
    lerp(&bc_cd, &bc, &cd, t);       /* point between bc and cd */
    lerp(&dest, &ab_bc, &bc_cd, t);   /* point on the bezier-curve */
    return dest.y;
}

uGamePad::PinMapping *uGamePad::getPinMapping() {
    return padMapping;
}
