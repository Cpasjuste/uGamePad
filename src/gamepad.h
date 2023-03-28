//
// Created by cpasjuste on 24/03/23.
//

#ifndef U_GAMEPAD_GAMEPAD_H
#define U_GAMEPAD_GAMEPAD_H

#include <cstdint>
#include "gamepad_data.h"
#include "devices.h"

#define MAX_BUTTONS 12
#define DEAD_ZONE 4000

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

    enum Led {
        LED_P1, LED_P2, LED_P3, LED_P4
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

    void setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance);

    const Device *getDevice() { return p_device; };

    void setLed(uint8_t type);

    bool update(const uint8_t *report, uint16_t len);

    PinMapping *getPinMapping();

    State &getState();

private:
    uint8_t m_addr = 0;
    uint8_t m_instance = 0;
    const Device p_device_unknown = {0x0000, 0x0000, (char *) "Unknown device", 0, TYPE_UNKNOWN};
    const Device *p_device = &p_device_unknown;
    State m_state;

    uint16_t getButtonsFromAxis(int x, int y);
};

extern uGamePad gamePad;

#endif /* U_GAMEPAD_GAMEPAD_H */
