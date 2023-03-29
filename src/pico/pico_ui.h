//
// Created by cpasjuste on 29/03/23.
//

#ifndef U_GAMEPAD_PICO_UI_H
#define U_GAMEPAD_PICO_UI_H

#include "ui.h"

namespace uGamePad {
    class PicoUi : public Ui {
    public:
        PicoUi();

        void flip() override;
    };
}

#endif //U_GAMEPAD_PICO_UI_H
