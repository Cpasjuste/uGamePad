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

        void setDevice(Device *device) { p_device = device; }

        Device *getDevice() { return p_device; }

        uint16_t getRepeatDelay() { return m_repeatDelayMs; }

        void setRepeatDelay(uint16_t ms) { m_repeatDelayMs = ms; }

        bool onHidReport(const uint8_t *report, uint16_t len);

    protected:
        Device *p_device = nullptr;
        uint16_t m_buttons{0};
        uint16_t m_buttons_prev{0};
        Clock m_repeatClock;
        uint16_t m_repeatDelayMs = 1000;
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
