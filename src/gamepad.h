//
// Created by cpasjuste on 24/03/23.
//

#ifndef U_GAMEPAD_GAMEPAD_H
#define U_GAMEPAD_GAMEPAD_H

#include <cstdint>
#include <string>
#include <vector>
#include "devices.h"
#include "utility/utility.h"
#include "utility/clock.h"

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
            DOWN = BIT(11),
            MENU = BIT(12),
            DELAY = BIT(13)
        };

        enum Mode {
            Jamma,
            Nes,
            Snes,
            Md
        };

        struct PinMapping {
            uint16_t button;
            uint8_t pin;
            int pinMode;
            int pinStatus;
        };

        struct Output {
            std::string name;
            Mode mode = Mode::Nes;
            std::vector<PinMapping> mappings;
        };

        GamePad();

        virtual void loop();

        virtual std::vector<GamePad::Output> getOutputModes() { return m_outputModes; };

        virtual Output *getOutputMode();

        virtual void setOutputMode(const Mode &mode);

        virtual void setOutputMode(const std::string &modeName);

        virtual uint16_t &getButtons() { return m_buttons; };

        void setDevice(const Device *device, uint8_t dev_addr, uint8_t instance);

        const Device *getDevice() { return p_device; };

        uint16_t getRepeatDelay() { return m_repeatDelayMs; };

        void setRepeatDelay(uint16_t ms) { m_repeatDelayMs = ms; };

    protected:
        uint8_t m_addr = 0;
        uint8_t m_instance = 0;
        const Device p_device_unknown = {0x0000, 0x0000, "Unknown device", nullptr};
        const Device *p_device = &p_device_unknown;
        uint16_t m_buttons{0};
        uint16_t m_buttons_prev{0};
        Clock m_repeatClock;
        uint16_t m_repeatDelayMs = 500;
        std::vector<GamePad::Output> m_outputModes;
        Mode m_outputMode = Mode::Jamma;

        ///
        /// axis handling
        ///
        Utility::Vec2i m_pa = {0, 0};
        Utility::Vec2i m_pb = {0, 0};
        Utility::Vec2i m_pc = {128, 32767};
        Utility::Vec2i m_pd = {128, 32767};

        int m_analog_map[256]{}; // map analog inputs to -32768 -> 32767 if needed

        static void lerp(Utility::Vec2i *dest, Utility::Vec2i *first, Utility::Vec2i *second, float t);

        int bezierY(float t);

        uint16_t getButtonsFromAxis(int x, int y, uint8_t type = ReportData::AxisType::AXIS_I16);

        static uint16_t getButtonsFromHat(int hat);
    };
}

#endif /* U_GAMEPAD_GAMEPAD_H */
