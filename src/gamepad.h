//
// Created by cpasjuste on 24/03/23.
//

#ifndef U_GAMEPAD_GAMEPAD_H
#define U_GAMEPAD_GAMEPAD_H

#include <cstdint>
#include <string>
#include <vector>
#include "devices.h"
#include "clock.h"
#include "utility.h"

#define TODO_NES_SNES_MD_CABLES

#define DEAD_ZONE 4000
#define GPIO_LOW 0
#define GPIO_HIGH 1
#define REPEAT_DELAY_DEFAULT 500

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
            DPAD_LEFT = BIT(8),
            DPAD_RIGHT = BIT(9),
            DPAD_UP = BIT(10),
            DPAD_DOWN = BIT(11),
            AXIS_LEFT = BIT(12),
            AXIS_RIGHT = BIT(13),
            AXIS_UP = BIT(14),
            AXIS_DOWN = BIT(15),
            MENU = BIT(16),
            DELAY = BIT(17),
            // Generic catch-all directions
            LEFT = DPAD_LEFT | AXIS_LEFT,
            RIGHT = DPAD_RIGHT | AXIS_RIGHT,
            UP = DPAD_UP | AXIS_UP,
            DOWN = DPAD_DOWN | AXIS_DOWN,
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
            bool direction;
            int defaultState;
        };

        struct Output {
            std::string name;
            Mode mode = Mode::Jamma;
            std::vector<PinMapping> mappings;
        };

        GamePad();

        virtual void loop();

        virtual std::vector<GamePad::Output> getOutputModes() { return m_outputModes; };

        virtual Output *getOutputMode();

        virtual void setOutputMode(const Mode &mode);

        virtual void setOutputMode(const std::string &modeName);

        virtual uint16_t &getButtons() { return m_buttons; }

        virtual bool onHidReport(const uint8_t *report, uint16_t len);

        void setDevice(Device *device) { p_device = device; }

        Device *getDevice() { return p_device; }

        uint16_t getRepeatDelay() { return m_repeatDelayMs; }

        void setRepeatDelay(uint16_t ms) { m_repeatDelayMs = ms; }

    protected:
        Device *p_device = nullptr;
        uint16_t m_buttons{0};
        uint16_t m_buttons_prev{0};
        Clock m_repeatClock;
        uint16_t m_repeatDelayMs = REPEAT_DELAY_DEFAULT;
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

        uint16_t getButtonsFromAxis(int x, int y, uint8_t type = 0/*ReportData::AxisType::AXIS_I16*/);

        static uint16_t getButtonsFromHat(int hat);
    };
}

#endif /* U_GAMEPAD_GAMEPAD_H */
