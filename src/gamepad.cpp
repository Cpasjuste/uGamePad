//
// Created by cpasjuste on 24/03/23.
//

#include <string>
#include <Arduino.h>
#include "tusb.h"
#include "main.h"
#include "gamepad.h"
#include "utility.h"

uGamePad gamePad;

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

// TODO: move this in devices.c/h
struct DS4Report {
    // https://www.psdevwiki.com/ps4/DS4-USB

    struct Button1 {
        inline static constexpr int SQUARE = 1 << 4;
        inline static constexpr int CROSS = 1 << 5;
        inline static constexpr int CIRCLE = 1 << 6;
        inline static constexpr int TRIANGLE = 1 << 7;
    };

    struct Button2 {
        inline static constexpr int L1 = 1 << 0;
        inline static constexpr int R1 = 1 << 1;
        inline static constexpr int L2 = 1 << 2;
        inline static constexpr int R2 = 1 << 3;
        inline static constexpr int SHARE = 1 << 4;
        inline static constexpr int OPTIONS = 1 << 5;
        inline static constexpr int L3 = 1 << 6;
        inline static constexpr int R3 = 1 << 7;
    };

    uint8_t reportID;
    uint8_t stickL[2];
    uint8_t stickR[2];
    uint8_t buttons1;
    uint8_t buttons2;
    uint8_t ps: 1;
    uint8_t tpad: 1;
    uint8_t counter: 6;
    uint8_t triggerL;
    uint8_t triggerR;
    // ...

    [[nodiscard]] int getHat() const { return buttons1 & 15; }
};

struct DS5Report {
    uint8_t reportID;
    uint8_t stickL[2];
    uint8_t stickR[2];
    uint8_t triggerL;
    uint8_t triggerR;
    uint8_t counter;
    uint8_t buttons[3];
    // ...

    struct Button {
        inline static constexpr int SQUARE = 1 << 4;
        inline static constexpr int CROSS = 1 << 5;
        inline static constexpr int CIRCLE = 1 << 6;
        inline static constexpr int TRIANGLE = 1 << 7;
        inline static constexpr int L1 = 1 << 8;
        inline static constexpr int R1 = 1 << 9;
        inline static constexpr int L2 = 1 << 10;
        inline static constexpr int R2 = 1 << 11;
        inline static constexpr int SHARE = 1 << 12;
        inline static constexpr int OPTIONS = 1 << 13;
        inline static constexpr int L3 = 1 << 14;
        inline static constexpr int R3 = 1 << 15;
        inline static constexpr int PS = 1 << 16;
        inline static constexpr int T_PAD = 1 << 17;
    };

    [[nodiscard]] int getHat() const { return buttons[0] & 15; }
};

struct XBOXReport {
    // https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/
    uint8_t type;
    uint8_t len;
    uint16_t buttons;
    uint8_t triggers[2];
    int16_t stickL[2];
    int16_t stickR[2];
    uint8_t unused[6];

    // is partsnotincluded wrong (not the same button order on my "Hori GEM Xbox controller" gamepad) ?
    struct Button {
        inline static constexpr int PAD_UP = 1 << 0;
        inline static constexpr int PAD_DOWN = 1 << 1;
        inline static constexpr int PAD_LEFT = 1 << 2;
        inline static constexpr int PAD_RIGHT = 1 << 3;
        inline static constexpr int START = 1 << 4;
        inline static constexpr int BACK = 1 << 5;
        inline static constexpr int R3 = 1 << 6;
        inline static constexpr int L3 = 1 << 7;
        inline static constexpr int LB = 1 << 8;
        inline static constexpr int RB = 1 << 9;
        inline static constexpr int GUIDE = 1 << 10; // TODO: not working
        inline static constexpr int A = 1 << 12;
        inline static constexpr int B = 1 << 13;
        inline static constexpr int X = 1 << 14;
        inline static constexpr int Y = 1 << 15;
    };
};

void uGamePad::setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance) {
    printf("uGamePad::setCurrentDevice: new gamepad discovered: %s\r\n", device->name);
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
        printf("uGamePad::update: device not set\r\n");
        return false;
    }

    if (p_device->type == TYPE_XBOX360) {
        //printf("uGamePad::update: %s (xbox)\r\n", p_device->name);
        if (sizeof(XBOXReport) == len && report[0] == 0x00) {
            auto r = reinterpret_cast<const XBOXReport *>(report);
            m_state.buttons =
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
            m_state.buttons |= uGamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1]);
            m_state.buttons |= uGamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1]);
            if (m_state.buttons != 0) printf("x360: %s\r\n", Utility::toString(m_state.buttons).c_str());
        } else {
            //printf("uGamePad::update: skipping report, wrong packet (xbox)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS4) {
        //printf("uGamePad::update: %s (ds4)\r\n", data->name);
        if (sizeof(DS4Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS4Report *>(report);
            m_state.axis[0] = r->stickL[0];
            m_state.axis[1] = r->stickL[1];
            m_state.buttons =
                    (r->buttons1 & DS4Report::Button1::CROSS ? uGamePad::Button::B_1 : 0) |
                    (r->buttons1 & DS4Report::Button1::CIRCLE ? uGamePad::Button::B_2 : 0) |
                    (r->buttons1 & DS4Report::Button1::SQUARE ? uGamePad::Button::B_3 : 0) |
                    (r->buttons1 & DS4Report::Button1::TRIANGLE ? uGamePad::Button::B_4 : 0) |
                    //(r->buttons1 & DS4Report::Button2::L1 ? uGamePad::Button::B_5 : 0) |
                    //(r->buttons1 & DS4Report::Button2::R1 ? uGamePad::Button::B_6 : 0) |
                    (r->buttons2 & DS4Report::Button2::SHARE ? uGamePad::Button::B_SELECT : 0) |
                    (r->tpad ? uGamePad::Button::B_SELECT : 0) |
                    (r->buttons2 & DS4Report::Button2::OPTIONS ? uGamePad::Button::B_START : 0);
            m_state.buttons |= uGamePad::getButtonsFromAxis(r->stickL[0], r->stickL[1]);
            m_state.buttons |= uGamePad::getButtonsFromAxis(r->stickR[0], r->stickR[1]);
            //m_state.hat = static_cast<uGamePad::Hat>(r->getHat());
            //gp.convertButtonsFromAxis(0, 1);
            //gp.convertButtonsFromHat();
            if (m_state.buttons != 0) printf("ds4: %s\r\n", Utility::toString(m_state.buttons).c_str());
        } else {
            //printf("uGamePad::update: skipping report, wrong packet (ds4)\r\n");
            return false;
        }
    } else if (p_device->type == TYPE_DS5) {
        //printf("uGamePad::update: %s (ds5)\r\n", data->name);
        if (sizeof(DS5Report) <= len && report[0] == 1) {
            auto r = reinterpret_cast<const DS5Report *>(report);
            auto buttons = r->buttons[0] | (r->buttons[1] << 8) | (r->buttons[2] << 16);
            m_state.axis[0] = r->stickL[0];
            m_state.axis[1] = r->stickL[1];
            m_state.buttons =
                    (buttons & DS4Report::Button1::CROSS ? uGamePad::Button::B_1 : 0) |
                    (buttons & DS4Report::Button1::CIRCLE ? uGamePad::Button::B_2 : 0) |
                    (buttons & DS4Report::Button1::TRIANGLE ? uGamePad::Button::B_3 : 0) |
                    (buttons & DS4Report::Button1::SQUARE ? uGamePad::Button::B_4 : 0) |
                    (buttons & DS4Report::Button2::L1 ? uGamePad::Button::B_5 : 0) |
                    (buttons & DS4Report::Button2::R1 ? uGamePad::Button::B_6 : 0) |
                    (buttons & DS4Report::Button2::SHARE ? uGamePad::Button::B_SELECT : 0) |
                    (buttons & DS4Report::Button2::OPTIONS ? uGamePad::Button::B_START : 0);
            //m_state.hat = static_cast<uGamePad::Hat>(r->getHat());
            //m_state.convertButtonsFromAxis(0, 1);
            //m_state.convertButtonsFromHat();
            if (m_state.buttons != 0) printf("ds5: %s\r\n", Utility::toString(m_state.buttons).c_str());
        } else {
            //printf("tuh_hid_report_received_cb: skipping report, wrong packet (ds5)\r\n");
            return false;
        }
    } else {
        return false;
    }

    return true;
}

uint16_t uGamePad::getButtonsFromAxis(int x, int y) {
    uint16_t buttons = 0;
    float slope = 0.414214f; // tangent of 22.5 degrees for size of angular zones
    auto analogX = (float) x, analogY = (float) y;

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

uGamePad::State &uGamePad::getState() {
    return m_state;
}

uGamePad::PinMapping *uGamePad::getPinMapping() {
    return padMapping;
}
