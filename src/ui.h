//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include <cstdint>
#include "utility/clock.h"
#include "menu.h"
#include "gamepad_settings.h"

namespace uGamePad {
    class Ui {
    public:
        enum MenuWidget {
            None,
            MainMenu,
            Splash,
            GamePadTest,
            Remap,
        };

        explicit Ui();

        void loop();

        void flip();

        void show(MenuWidget menuWidget);

        bool isVisible() { return p_screen->isVisible(); };

    private:
        Widget *p_screen;
        Menu *p_menu;
        Bitmap *p_splash;
        Text *p_splashText;
        GamePadSettings *p_gamePadInfo;
        MenuWidget m_menuCurrent = None;
        Clock m_triggerMenuClock;
    };
}

#endif //U_GAMEPAD_UI_H
