//
// Created by cpasjuste on 28/03/23.
//

#ifndef UGAMEPAD_DEVICES_H
#define UGAMEPAD_DEVICES_H

#include <cstdint>
#include "gamepad_data.h"

struct XBOXReport {
    // https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/
    uint8_t type;
    uint8_t len;
    uint16_t buttons;
    uint8_t triggers[2];
    int16_t stickL[2];
    int16_t stickR[2];
    uint8_t unused[6];
    // ...

    // is "partsnotincluded" wrong (not the same button order on my "Hori GEM Xbox controller" gamepad) ?
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

struct DS4Report {
    // https://www.psdevwiki.com/ps4/DS4-USB
    uint8_t reportID;
    uint8_t stickL[2];
    uint8_t stickR[2];
    uint8_t buttons1;
    uint8_t buttons2;
    uint8_t ps: 1;
    uint8_t t_pad: 1;
    uint8_t counter: 6;
    uint8_t triggerL;
    uint8_t triggerR;
    // ...

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
};

const Device *find_device(uint16_t vid, uint16_t pid);

#endif //UGAMEPAD_DEVICES_H
