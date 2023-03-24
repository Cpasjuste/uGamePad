/*
 * author : Shuichi TAKANO
 * since  : Fri Jul 30 2021 04:42:27
 */
#ifndef UGAMEPAD_GAMEPAD_H
#define UGAMEPAD_GAMEPAD_H

namespace io
{
    struct GamePadState
    {
        struct Button
        {
            inline static constexpr int A = 1 << 0;
            inline static constexpr int B = 1 << 1;
            inline static constexpr int X = 1 << 2;
            inline static constexpr int Y = 1 << 3;
            inline static constexpr int SELECT = 1 << 6;
            inline static constexpr int START = 1 << 7;

            inline static constexpr int LEFT = 1 << 31;
            inline static constexpr int RIGHT = 1 << 30;
            inline static constexpr int UP = 1 << 29;
            inline static constexpr int DOWN = 1 << 28;
        };

        enum class Hat
        {
            N,
            NE,
            E,
            SE,
            S,
            SW,
            W,
            NW,
            RELEASED,
        };

        uint8_t axis[3]{0x80, 0x80, 0x80};
        Hat hat{Hat::RELEASED};
        uint32_t buttons{0};

    public:
        void convertButtonsFromAxis(int axisX, int axisY) {};
        void convertButtonsFromHat() {};
    };

    GamePadState state;

    GamePadState &getCurrentGamePadState(int i) { return state; };
}

#endif /* UGAMEPAD_GAMEPAD_H */
