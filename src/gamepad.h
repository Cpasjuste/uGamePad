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

#define GAMEPAD_DZ 8000
#define GPIO_FLOAT (-1)
#define GPIO_LOW 0
#define GPIO_HIGH 1
#define REPEAT_DELAY_DEFAULT UINT16_MAX

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
            AXIS_L_LEFT = BIT(12),
            AXIS_L_RIGHT = BIT(13),
            AXIS_L_UP = BIT(14),
            AXIS_L_DOWN = BIT(15),
            AXIS_R_LEFT = BIT(16),
            AXIS_R_RIGHT = BIT(17),
            AXIS_R_UP = BIT(18),
            AXIS_R_DOWN = BIT(19),
            MENU = BIT(20),
            DELAY = BIT(21),
            // Generic catch-all directions
            LEFT = DPAD_LEFT | AXIS_L_LEFT | AXIS_R_LEFT,
            RIGHT = DPAD_RIGHT | AXIS_L_RIGHT | AXIS_R_RIGHT,
            UP = DPAD_UP | AXIS_L_UP | AXIS_R_UP,
            DOWN = DPAD_DOWN | AXIS_L_DOWN | AXIS_R_DOWN,
        };

        enum Mode {
            Jamma,
            Nes,
            Snes,
            Md
        };

        struct PinMapping {
            uint32_t button;
            uint8_t pin;
            bool direction;
            int defaultState;
        };

        struct Output {
            std::string name;
            Mode mode = Jamma;
            std::vector<PinMapping> mappings;
        };

        GamePad();

        virtual ~GamePad() = default;

        virtual void loop();

        virtual std::vector<Output> getOutputModes() { return m_outputModes; };

        virtual Output *getOutputMode();

        virtual void setOutputMode(const Mode &mode);

        virtual void setOutputMode(const std::string &modeName);

        virtual uint32_t &getButtons() { return m_buttons; }

        virtual bool onHidReport(const uint8_t *report, uint16_t len);

        // wait for buttons to be released
        virtual void flush();

        // clear buttons states immediately
        virtual void clear() { m_buttons = 0; }

        void setDevice(Device *device) {
            p_device = device;
            if (p_device) p_deviceDefaults = get_device(p_device->vid, p_device->pid);
        }

        void setDeviceDefaultDescriptor() const {
            if (p_device && p_deviceDefaults) {
                memcpy(p_device->report, p_deviceDefaults->report, sizeof(InputReportDescriptor));
            }
        }

        Device *getDevice() const { return p_device; }

        Device *getDeviceDefaults() const { return p_deviceDefaults; }

        uint16_t getRepeatDelay() const { return m_repeatDelayMs; }

        void setRepeatDelay(const uint16_t ms) { m_repeatDelayMs = ms; }

        static uint8_t getButtonIndex(uint32_t button);

    protected:
        Device *p_device = nullptr;
        Device *p_deviceDefaults = nullptr;
        uint32_t m_buttons{0};
        uint32_t m_buttons_prev{0};
        Clock m_repeatClock;
        uint16_t m_repeatDelayMs = REPEAT_DELAY_DEFAULT;
        std::vector<Output> m_outputModes;
        Mode m_outputMode = Jamma;

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

        uint32_t getButtonsFromAxis(int x, int y, uint8_t type = AXIS_TYPE_U8 | AXIS_TYPE_LEFT) const;

        static uint32_t getButtonsFromHat(int hat);
    };
}

#endif /* U_GAMEPAD_GAMEPAD_H */
