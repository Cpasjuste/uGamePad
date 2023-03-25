//
// Created by cpasjuste on 24/03/23.
//

#ifndef U_GAMEPAD_GAMEPAD_H
#define U_GAMEPAD_GAMEPAD_H

#include <cstdint>

#define MAX_BUTTONS 12

class uGamePad {
public:
    enum Button {
        B_1 = 1 << 0,
        B_2 = 1 << 1,
        B_3 = 1 << 2,
        B_4 = 1 << 3,
        B_5 = 1 << 4,
        B_6 = 1 << 5,
        B_START = 1 << 6,
        B_SELECT = 1 << 7,
        B_LEFT = 1 << 8,
        B_RIGHT = 1 << 9,
        B_UP = 1 << 10,
        B_DOWN = 1 << 11
    };

    enum Hat {
        N, NE, E, SE,
        S, SW, W, NW,
        RELEASED,
    };

    struct State {
        uint16_t buttons{0};
        Hat hat{Hat::RELEASED};
        uint8_t axis[3]{0x80, 0x80, 0x80};
    public:
        void convertButtonsFromAxis(int axisX, int axisY) {};

        void convertButtonsFromHat() {};
    };

    struct PinMapping {
        uint16_t button;
        pin_size_t pin;
    };

    static PinMapping *getPinMapping();

    static State &getState();
};

#endif /* U_GAMEPAD_GAMEPAD_H */
