//
// Created by cpasjuste on 18/04/23.
//

#ifndef U_GAMEPAD_CLOCK_H
#define U_GAMEPAD_CLOCK_H

#include "clock_time.h"

namespace uGamePad {
    class Clock {
    public:
        Clock();

        [[nodiscard]] Time getCurrentTime() const;

        [[nodiscard]] Time getElapsedTime() const;

        Time restart();

    protected:
        Time m_startTime;
    };
}

#endif //U_GAMEPAD_CLOCK_H
