//
// Created by cpasjuste on 24/03/23.
//

#ifndef U_GAMEPAD_GAMEPAD_H
#define U_GAMEPAD_GAMEPAD_H

#include <cstdint>
#include <vector>
#include "devices.h"

#define MAX_BUTTONS 12
#define MAX_AXIS 4
#define DEAD_ZONE 4000

namespace uGamePad {
    class GamePad {
    public:
        enum Button {
            B1 = BIT(0),
            B2 = BIT(1),
            B3 = BIT(2),
            B4 = BIT(3),
            B5 = BIT(4),
            B6 = BIT(5),
            START = BIT(6),
            SELECT = BIT(7),
            LEFT = BIT(8),
            RIGHT = BIT(9),
            UP = BIT(10),
            DOWN = BIT(11)
        };

        enum Led {
            LED_P1, LED_P2, LED_P3, LED_P4
        };

        enum Mode {
            Mvs,
            Nes
        };

        struct PinMapping {
            uint16_t button;
            uint8_t pin;
            int pinMode;
            int pinStatus;
        };

        struct Output {
            Mode mode = Mode::Nes;
            std::vector<PinMapping> mappings;
        };

        GamePad();

        void setCurrentDevice(const Device *device, uint8_t dev_addr, uint8_t instance);

        const Device *getDevice() { return p_device; };

        virtual void setLed(uint8_t type) {};

        virtual bool update(const uint8_t *report, uint16_t len) { return false; };

        virtual Output *getOutputMode() { return nullptr; };

        uint16_t &getButtons() { return m_buttons; };

    protected:
        uint8_t m_addr = 0;
        uint8_t m_instance = 0;
        const Device p_device_unknown = {0x0000, 0x0000, "Unknown device", nullptr};
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

        uint16_t getButtonsFromAxis(int x, int y, uint8_t type = ReportData::AxisType::AXIS_I16);

        static uint16_t getButtonsFromHat(int hat);
    };
}

#endif /* U_GAMEPAD_GAMEPAD_H */
