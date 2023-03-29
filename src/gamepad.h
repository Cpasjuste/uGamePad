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

    enum AxisType : uint8_t {
        AXIS_32767 = 0,
        AXIS_255 = 4,
        AXIS_FLIP_Y = 8
    };

    enum Led {
        LED_P1, LED_P2, LED_P3, LED_P4
    };

    struct PinMapping {
        uint16_t button;
        pin_size_t pin;
    };

    uGamePad();

    void setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance);

    const Device *getDevice() { return p_device; };

    void setLed(uint8_t type);

    bool update(const uint8_t *report, uint16_t len);

    PinMapping *getPinMapping();

    uint16_t &getButtons() { return m_buttons; };

private:
    uint8_t m_addr = 0;
    uint8_t m_instance = 0;
    const Device p_device_unknown = {0x0000, 0x0000, (char *) "Unknown device", 0, TYPE_UNKNOWN};
    const Device *p_device = &p_device_unknown;
    uint16_t m_buttons{0};

    ///
    /// axis handling
    ///
    typedef struct {
        int x;
        int y;
    } point;

    point m_pa = {0, 0};
    point m_pb = {0, 0};
    point m_pc = {128, 32767};
    point m_pd = {128, 32767};

    int m_analog_map[256]{}; // map analog inputs to -32768 -> 32767 if needed

    static void lerp(point *dest, point *first, point *second, float t);

    int calc_bezier_y(float t);

    uint16_t getButtonsFromAxis(int x, int y, uint8_t type = AXIS_32767);

    static uint16_t getButtonsFromHat(int hat);
};

extern uGamePad gamePad;

#endif /* U_GAMEPAD_GAMEPAD_H */
