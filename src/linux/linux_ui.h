//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_LINUX_UI_H
#define U_GAMEPAD_LINUX_UI_H

#include "ui.h"

namespace uGamePad {
    class LinuxUi : public Ui {
    public:
        LinuxUi();

        void flip() override;
    };
}

#endif //U_GAMEPAD_LINUX_UI_H
