//
// Created by cpasjuste on 30/03/23.
//

#ifndef U_GAMEPAD_UI_H
#define U_GAMEPAD_UI_H

#include "utility/clock.h"
#include "menu.h"
#include "system_info.h"
#include "gamepad_settings.h"
#include "profiles.h"
#include "messagebox.h"

namespace uGamePad {
    class Ui {
    public:
        enum MenuWidget {
            None,
            MainMenu,
            Splash,
            Profiles,
            GamePadTest,
            Remap,
            Info,
        };

        explicit Ui();

        void loop();

        void flip();

        void show(MenuWidget menuWidget);

        bool isVisible() { return p_screen->isVisible(); };

        MessageBox *getMessageBox() const { return p_messageBox; };

    private:
        Widget *p_screen;
        Menu *p_menu;
        Bitmap *p_splash;
        Text *p_splashText;
        GamePadSettings *p_gamePadInfo;
        SystemInfo *p_systemInfo;
        UiProfiles *p_profiles;
        MessageBox *p_messageBox;
        MenuWidget m_menuCurrent = None;
        Clock m_triggerMenuClock;
    };
}

#endif //U_GAMEPAD_UI_H
