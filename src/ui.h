//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include <cstdint>
#include "rectangle.h"
#include "gamepad_info.h"

namespace uGamePad {
    class Ui {
    public:
        explicit Ui();

        void loop();

        void showSplash();

        bool isActive();

    private:
        Widget *p_screen;
        Bitmap *p_splash;
        Text *p_splash_text;
        GamePadInfo *p_gamePadInfo;
    };
}

#endif //U_GAMEPAD_UI_H
