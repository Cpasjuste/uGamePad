//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include <cstdint>
#include "utility/clock.h"
#include "rectangle.h"
#include "gamepad_info.h"

namespace uGamePad {
    class Ui {
    public:
        explicit Ui();

        void loop();

        bool isVisible() { return p_screen->isVisible(); };

    private:
        Widget *p_screen;
        Bitmap *p_splash;
        Text *p_splashText;
        GamePadInfo *p_gamePadInfo;
        Clock m_triggerMenuClock;
    };
}

#endif //U_GAMEPAD_UI_H
